// testshapes demo for Adafruit RGBmatrixPanel library.
// Demonstrates the drawing abilities of the RGBmatrixPanel library.
// For 16x32 RGB LED matrix:
// http://www.adafruit.com/products/420

// Written by Limor Fried/Ladyada & Phil Burgess/PaintYourDragon
// for Adafruit Industries.
// BSD license, all text above must be included in any redistribution.

#include "pitches.h"
#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#include "TaskScheduler.h" //provided by at https://github.com/arkhipenko/TaskScheduler
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#define CLK 8  // MUST be on PORTB!
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

int sensorValue = 101;
bool EMERGENCY = false;
int count = 0;
//int pos = 0;
int disp1 = 0; 
int msg[1];
RF24 radio(0,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;

//create callback methods
void Distance_Callback();
void DisplayMatrix_Callback();

//Tasks
Task DisplayMatrix(500, TASK_FOREVER, &DisplayMatrix_Callback);
Task Distance(100, TASK_FOREVER, &Distance_Callback);

Scheduler runner;

#define inits 0
#define slow 1
#define right 2
#define left 3
#define normal 4
#define none 5
static int state = inits;

void DisplayMatrix_Callback(){  
  //read from SPI
  disp1 = 0;

  if (radio.available()){
   bool done = false;    
   while (!done){
     done = radio.read(msg, 1);      
     disp1 = msg[0];
     Serial.println(msg[0]);
     delay(10);}
 }
 else{Serial.println("No radio available");}

  switch(state){  //ACTIONS
    case inits:
      break;
    case slow:
      //          SLOW SIGNAL     //
      matrix.setCursor(2, 0);   // start at top left, with one pixel of spacing
      matrix.setTextSize(1);    // size 1 == 8 pixels high
    
      // print each letter with a rainbow color
      matrix.setTextColor(matrix.Color333(4,0,0));
      matrix.print('S');
      matrix.setTextColor(matrix.Color333(4,0,0));
      matrix.print('L');
      matrix.setTextColor(matrix.Color333(4,0,0));
      matrix.print('O');
      matrix.setTextColor(matrix.Color333(4,0,0));
      matrix.print('W');
      matrix.setTextColor(matrix.Color333(4,0,0));
      matrix.print('!');

      matrix.setCursor(2, 8);   // start at top left, with one pixel of spacing
      matrix.setTextSize(1);    // size 1 == 8 pixels high
    
      // print each letter with a rainbow color
      matrix.setTextColor(matrix.Color333(4,0,0));
      matrix.print('D');
      matrix.setTextColor(matrix.Color333(4,0,0));
      matrix.print('O');
      matrix.setTextColor(matrix.Color333(4,0,0));
      matrix.print('W');
      matrix.setTextColor(matrix.Color333(4,0,0));
      matrix.print('N');
      matrix.setTextColor(matrix.Color333(4,0,0));
      matrix.print('!');
      /*
      if(pos > 30) pos = 0;
      else pos++;

      delay(100);
      matrix.fillScreen(matrix.Color333(0, 0, 0));
      */
      if(EMERGENCY) tone(A4, NOTE_A6, 250);
      break;
    case right:
      //        Right Turn      //
      matrix.drawLine(15, 0, 31, 8, matrix.Color333(7, 0, 0));
      matrix.drawLine(16, 0, 31, 7, matrix.Color333(7, 0, 0));
      matrix.drawLine(17, 0, 31, 6, matrix.Color333(7, 0, 0));
      matrix.drawLine(15, 14, 31, 6, matrix.Color333(7, 0, 0));
      matrix.drawLine(16, 14, 31, 7, matrix.Color333(7, 0, 0));
      matrix.drawLine(17, 14, 31, 8, matrix.Color333(7, 0, 0));
    
      matrix.fillRect(0, 6, 32, 3, matrix.Color333(7, 0, 0));
      break;
    case left:
      //        Left Turn      //
      matrix.drawLine(15, 0, 0, 8, matrix.Color333(7, 0, 0));
      matrix.drawLine(16, 0, 0, 7, matrix.Color333(7, 0, 0));
      matrix.drawLine(17, 0, 0, 6, matrix.Color333(7, 0, 0));
      matrix.drawLine(15, 14, 0, 6, matrix.Color333(7, 0, 0));
      matrix.drawLine(16, 14, 0, 7, matrix.Color333(7, 0, 0));
      matrix.drawLine(17, 14, 0, 8, matrix.Color333(7, 0, 0));
    
      matrix.fillRect(0, 6, 32, 3, matrix.Color333(7, 0, 0));
      break;
    case normal:
      matrix.fillRect(7, 4, 18, 8, matrix.Color333(7, 0, 0));
      break;
    case none:
      matrix.fillScreen(matrix.Color333(0, 0, 0));
      noTone(A4);
      break;
  }

  switch(state){  //STATE CHANGING
    case inits:
      state = none;
      break;
    case slow:
      //          SLOW SIGNAL     //
      state = none;
      count = 0;
      break;
    case right:
      //          RIGHT SIGNAL     //
      state = none;
      break;
    case left:
      //          LEFT SIGNAL     //
      state = none;
      break;
    case normal:
      //          DEFAULT SIGNAL     //
      state = none;
      break;
    case none:
      //          NO SIGNAL     //
      if((disp1 == 3) || EMERGENCY) state = slow;
      else if(disp1 == 2) state = right;
      else if(disp1 == 1) state = left;
      else if(disp1 == 5) state = normal;
      break;
  }
}

void Distance_Callback(){  
  sensorValue = analogRead(A5);
  //Serial.println(sensorValue);

  if(sensorValue < 100){
    if(count < 2) count++;
    else {
      EMERGENCY = true;
      count = 0;
    }
  }
  else {
    if(count < 3) count++;
    else {
      EMERGENCY = false;
      count = 0;
    }
  }
}

void setup() {
  //set rate to 9600bps
  Serial.begin(9600); //set rate to read from distance sensor
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();

  Serial.println("Scheduler TEST");
  runner.init();
  Serial.println("Initialized scheduler");
  
  runner.addTask(Distance);
  Serial.println("added t1");
  runner.addTask(DisplayMatrix);
  Serial.println("added t2");

  delay(2000);
  
  Distance.enable();
  Serial.println("Enabled t1");
  DisplayMatrix.enable();
  Serial.println("Enabled t2");

  matrix.begin();
  // fill the screen with 'black'
  matrix.fillScreen(matrix.Color333(0, 0, 0));
}

void loop() { 
  runner.execute();
  /*
 if (radio.available()){
   bool done = false;    
   while (!done){
     done = radio.read(msg, 1);      
     Serial.println(msg[0]);
     delay(10);}
 }
 else{Serial.println("No radio available");}
 */
}



/*
  if((disp1 == HIGH && disp2 == HIGH) || (EMERGENCY)){
    //          SLOW SIGNAL     //
    matrix.setCursor(5 - pos, 1);   // start at top left, with one pixel of spacing
    matrix.setTextSize(2);    // size 1 == 8 pixels high
  
    // print each letter with a rainbow color
    matrix.setTextColor(matrix.Color333(4,0,0));
    matrix.print('S');
    matrix.setTextColor(matrix.Color333(4,0,0));
    matrix.print('L');
    matrix.setTextColor(matrix.Color333(4,0,0));
    matrix.print('O');
    matrix.setTextColor(matrix.Color333(4,0,0));
    matrix.print('W');
    if(pos > 30) pos = 0;
    else pos++;
  
    delay(100);
    matrix.fillScreen(matrix.Color333(0, 0, 0));
  }

  else if(disp1 == HIGH && disp2 == LOW){
    //        Right Turn      //
    matrix.drawLine(15, 0, 31, 8, matrix.Color333(7, 0, 0));
    matrix.drawLine(16, 0, 31, 7, matrix.Color333(7, 0, 0));
    matrix.drawLine(17, 0, 31, 6, matrix.Color333(7, 0, 0));
    matrix.drawLine(15, 14, 31, 6, matrix.Color333(7, 0, 0));
    matrix.drawLine(16, 14, 31, 7, matrix.Color333(7, 0, 0));
    matrix.drawLine(17, 14, 31, 8, matrix.Color333(7, 0, 0));
  
    matrix.fillRect(0, 6, 32, 3, matrix.Color333(7, 0, 0));
    
    delay(500);
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    delay(500);
  }

  else if(disp1 == LOW && disp2 == HIGH){
    //        Left Turn      //
    matrix.drawLine(15, 0, 0, 8, matrix.Color333(7, 0, 0));
    matrix.drawLine(16, 0, 0, 7, matrix.Color333(7, 0, 0));
    matrix.drawLine(17, 0, 0, 6, matrix.Color333(7, 0, 0));
    matrix.drawLine(15, 14, 0, 6, matrix.Color333(7, 0, 0));
    matrix.drawLine(16, 14, 0, 7, matrix.Color333(7, 0, 0));
    matrix.drawLine(17, 14, 0, 8, matrix.Color333(7, 0, 0));
  
    matrix.fillRect(0, 6, 32, 3, matrix.Color333(7, 0, 0));
    
    delay(500);
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    delay(500);
  }

  else  matrix.fillScreen(matrix.Color333(0, 0, 0));  //draw square in middle
  */


  
/*
  switch(state){  //STATE CHANGING
    case inits:
      state = none;
      count = 0;
      break;
    case slow:
      //          SLOW SIGNAL     //
      if((disp1 == 333) ) state = slow;
      else state = none;
      count = 0;
      break;
    case right:
      //          RIGHT SIGNAL     //
      count++;
      if(count >= 5) {
        state = right;
        count = 0;
      }
      else state = none;
      break;
    case left:
      //          LEFT SIGNAL     //
      count++;
      if(count >= 5) {
        state = left;
        count = 0;
      }
      else state = none;
      break;
    case normal:
      //          DEFAULT SIGNAL     //
      count++;
      if(count >= 5) {
        state = normal;
        count = 0;
      }
      else state = none;
      break;
    case none:
      //          NO SIGNAL     //
      count++;
      if((disp1 == 3)) state = slow;
      else if(disp1 == 2 && count >= 5) {
        state = right;
        count = 0;
      }
      else if(disp1 == 1 && count >= 5) {
        state = left;
        count = 0;
      }
      else if(count >= 5) {
        state = normal;
        count = 0;
      }
      else state = none;
      break;
  } */
