This specific guide utilizes two arduino unos to work, other microcontrollers might have to be adjusted.

#### Front Arduino:

  LCD:
  
    RS - A3
    Enable - PIN 3
    D4 - PIN 4
    D5 - PIN 5
    D6 - PIN 6
    D7 - PIN 7
  
  Photoresistor: A0 input, PIN 8 output
  
  Left Switch: A4
  
  Right Switch: A5
  
  Transceiver:
  
    CE - PIN 9
    CS(N) - PIN 10
    MOSI - PIN 11
    MISO - PIN 12
    SCK - PIN 13

  Hall effect sensor: PIN 2

#### Back Arduino:

  LED Matrix:
    Follow the hookup guide here: https://learn.sparkfun.com/tutorials/rgb-panel-hookup-guide
    But with Clock on 8 and Latch on A3
    
  Transceiver:
  
    CS(N) - PIN 10
    MOSI - PIN 11
    MISO - PIN 12
    SCK - PIN 13
     
  Speaker: A4
  
  Ultrasonic Distance Sensor: A5
