# SparCycle

This project is a smartbike that has several safety features integrated into it. These features includes:
* a built in flashlight that automatically turns on in darkness or when manually switched on
* two switches to signal left and right
* an ultrasonic distance sensor that indicates if something is too close to the back
* a LED matrix that displays left and right signaling as well as an emergency display
* a Hall effect sensor that helps measure the speed of the bike
* an LCD screen that displays the speed in MPH and indicates whether the signal switches are on

When hooking up the Hall effect sensor, make sure to have it close enough for it to read the magnet. This may vary depending on the type and strength of the magnet.

Special components and technologies used in this project include: 
* Two Arduino Unos and the Arduino IDE
* Ultrasonic Range Finder - LV-MaxSonar-EZ3 (https://www.sparkfun.com/products/8501)
* 16x32 RGB LED Panel (https://www.sparkfun.com/products/12583)
* nRF24L01+ 2.4GHz Wireless Transceiver (http://www.amazon.com/nRF24L01-Wireless-Transceiver-Antistatic-Compatible/dp/B00E594ZX0)
* Hall effect sensor and magnets (http://www.amazon.com/Sunkee-A3144E-OH3144E-Effect-Sensor/dp/B00ATNJH20/)
* LCD Display
* A bicycle, lots of tape, and a box.

#####Demo Video: https://youtu.be/-bI9D3a9aBY 

#####Links:
* https://github.com/adafruit/RGB-matrix-Panel - Library used to control LED Panel
* https://github.com/arkhipenko/TaskScheduler - Library used to create a task scheduler in the Arduino IDE.
* http://forum.arduino.cc/index.php?topic=138663.0 - Link used to get the transceiver working
* www.github.com/maniacbug/RF24 - Library used to get the transceiver working
* http://diyhacking.com/arduino-hall-effect-sensor-tutorial/ - Tutorial used to setup and test the Hall effect sensor.

Collaboration: I had Chandler Morehouse help me with the wiring and serial communication.
