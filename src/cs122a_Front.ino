#include <LiquidCrystal.h>
#include "pitches.h"
#include "TaskScheduler.h" //provided by at https://github.com/arkhipenko/TaskScheduler
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

//initialize lcd pins
//LiquidCrystal lcd(12, 11, 2, 3, 4, 5);
LiquidCrystal lcd(A3, 3, 4, 5, 6, 7);

// value read from the photoresistor
int sensorValue = 0;

//transiever intialization
int msg[1];
RF24 radio(9, 10);
const uint64_t pipe = 0xE8E8F0F0E1LL;
//int SW1 = 3;

//signal variables
int leftSignal, rightSignal;

//hall effect initializations
volatile byte half_revolutions;
unsigned int rpm;
unsigned long timeold;

//create callback methods
void DisplayLCD_Callback();
void FlashLight_Callback();
//void SensorRead_Callback();
void DisplayMatrix_Callback();
void Hall_Callback();

//Tasks
Task DisplayLCD(100, TASK_FOREVER, &DisplayLCD_Callback);
Task FlashLight(3000, TASK_FOREVER, &FlashLight_Callback);
//Task SensorRead();
Task DisplayMatrix(100, TASK_FOREVER, &DisplayMatrix_Callback);
Task Hall(1, TASK_FOREVER, &Hall_Callback);

Scheduler runner;

void DisplayLCD_Callback() {
  lcd.setCursor(5, 0);
  // print the mph
  double mph = rpm*188.4;
  mph = mph/5280;
  lcd.print(mph);

  // print the if rightSignal is on
  lcd.setCursor(2,1);
  if(rightSignal == HIGH) lcd.print('R');
  else lcd.print(' ');

  // print the if leftSignal is on
  lcd.setCursor(0,1);
  if(leftSignal == HIGH) lcd.print('L');
  else lcd.print(' ');
    
}
void FlashLight_Callback() {
  // read the analog in value:
  sensorValue = analogRead(A0);
  // map it to the range of the analog out:

  // print the results to the serial monitor:
  Serial.print("sensor = " );
  Serial.print(sensorValue);
  Serial.print("\n");

  if (sensorValue < 200) {
    digitalWrite(8, HIGH);
  }
  else digitalWrite(8, LOW);

  // wait 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  //delay(50);

}
void DisplayMatrix_Callback() {
  rightSignal = digitalRead(A4);
  leftSignal = digitalRead(A5);

  if(rightSignal == HIGH && leftSignal == HIGH){
    //Serial.println("both");
    msg[0] = 3;
    radio.write(msg, 1);
  }
  else if(rightSignal == HIGH && leftSignal == LOW){
    //Serial.println("right");
    msg[0] = 2;
    radio.write(msg, 1);
  }
  else if(rightSignal == LOW && leftSignal == HIGH){
    //Serial.println("left");
    msg[0] = 1;
    radio.write(msg, 1);
  }
  else{
    //Serial.println("none");
    msg[0] = 5;
    radio.write(msg, 1);
  }
}

void Hall_Callback() {
  if (half_revolutions >= 20) { 
     rpm = 30*1000/(millis() - timeold)*half_revolutions;
     timeold = millis();
     half_revolutions = 0;
     Serial.print("RPM: ");
     Serial.println(rpm,DEC);
   }
}

void magnet_detect()//This function is called whenever a magnet/interrupt is detected by the arduino
 {
   half_revolutions++;
   Serial.println("detect");
 }

void setup() {
  //set rate to 9600bps
  Serial.begin(9600);

  //start transiever code
  radio.begin();
  radio.openWritingPipe(pipe);

  pinMode(8, OUTPUT); //set pin for flashlight
  pinMode(A4, INPUT); //set pin for right signal
  pinMode(A5, INPUT); //set pin for left signal

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("MPH: ");

  attachInterrupt(0, magnet_detect, RISING);//Initialize the intterrupt pin (Arduino digital pin 2)
  half_revolutions = 0;
  rpm = 0;
  timeold = 0;

  //start scheduler code
  Serial.println("Scheduler TEST");
  runner.init();
  Serial.println("Initialized scheduler");

  runner.addTask(DisplayLCD);
  Serial.println("added t1");
  runner.addTask(FlashLight);
  Serial.println("added t2");
  runner.addTask(DisplayMatrix);
  Serial.println("added t3");
  runner.addTask(Hall);
  Serial.println("added t4");

  delay(2000);

  DisplayLCD.enable();
  Serial.println("Enabled t1");
  FlashLight.enable();
  Serial.println("Enabled t2");
  DisplayMatrix.enable();
  Serial.println("Enabled t3");
  Hall.enable();
  Serial.println("Enabled t4");
}

void loop() {
  runner.execute();
}

