
/*
 * This is the codes for the controller which was developed to synchronize light activation for optogenetics 
 * experiments with mobile phone video capture to record the behavior of transgenic Drosophila melanogaster. 
 * The controller included a Status LED Module, Optogenetic Stimulation LED Module and Motion Sensor Module. 
 * All components, other than the optogenetic stimulation LED (627nm, LUXEON Rebel LED, LuxeonStar), 
 * came from an ELEGOO UNO Project Super Starter Kit. 
 * 
 * Modified on August 3th 2020
 * by Liudi Luo
 * Catherine von Reyn lab
 * 
 * The lightweight Low Power library was obtained from Rocket Scream Electronics
 * https://github.com/rocketscream/Low-Power
 */


/* Include the Low Power Library */
#include "LowPower.h"

/* Define Pins */
// use pin 11 as trigger pin and pin 12 as echo pin for HC-SR04 (Motion Sensor Module)
int trigPin = 11; 
int echoPin = 12;

// use pin 7 for LUXEON Rebel LED (Optogenetic Stimulation LED Module)
int ledPin = 7;

// use pin 3 and 2 for the two push switches (Status LED Module)
// they are used to switch the development board into the ready state or dormant state
int buttonApin = 3;
int buttonBpin = 2;
// use pin 6, pin 5 and pin 4 for the red, green and blue of RGB LED (Status LED Module)
int RED = 6;
int GREEN = 5;
int BLUE = 4;

long duration, cm;

void setup() {
  /* Initialization Settings */
   // Initialize serial connection and sets baud rate to 9600
   Serial.begin(9600);
   // Initialize trigger pin as output and echo pin as input
   pinMode(trigPin, OUTPUT);
   pinMode(echoPin, INPUT);
   // Initialize LUXEON Rebel LED pin as output and keep it off
   pinMode(ledPin, OUTPUT);
   digitalWrite(ledPin, LOW); 
   // Initialize the two switches pins as input and pull up to HIGH
   pinMode(buttonApin, INPUT_PULLUP);  
   pinMode(buttonBpin, INPUT_PULLUP); 
   // Initialize the red, green and blue pins of RGB LED as output and keep them off
   pinMode(RED, OUTPUT);
   pinMode(GREEN, OUTPUT);
   pinMode(BLUE, OUTPUT);
   digitalWrite(RED, LOW);
   digitalWrite(GREEN, LOW);
   digitalWrite(BLUE, LOW);
   // allow two switches pins to trigger interupt to switch the status of board
   attachInterrupt(digitalPinToInterrupt(3),TurnDeviceOn,CHANGE);
   attachInterrupt(digitalPinToInterrupt(2),TurnDeviceOff,CHANGE);
}

void loop() {
  /* Perform Different Functions According to the State of the Development Board */
  // detect whether the board is on dormant state
  if (digitalRead(GREEN) == LOW)
  {
    // Enter power down state with ADC and BOD module disabled.
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 
  }
  
  // detect whether the board is on ready state
  if (digitalRead(GREEN) == HIGH)
   {
    // generat 50kHz waves for transmitter
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // detect the time it takes for a wave to be sent and received
    duration = pulseIn(echoPin, HIGH); 
    // calculate the distance of the object from the transmitter 
    cm = (duration/2) / 29.1;
    Serial.print(cm);
    Serial.print("cm");
    Serial.println();
    // when a object is less than 10 cm from transmitter, which means 
    // the finger press the shutter of the camera
    if (cm<10)
   {
    digitalWrite(BLUE, HIGH); // ture on blue led
    delay(1500); // wait 1.5s
    int F;
      for (int F=1; F <= 1; F=F+1)
      {
        digitalWrite(ledPin, HIGH);  // LUXEON Rebel LED on   
        delay(300);  // keep for 0.3s    
        digitalWrite(ledPin, LOW);  // LUXEON Rebel LED off
        delay(30000);  // keep for 30s
        digitalWrite(BLUE, LOW); //ture off blue led
      }
    }
   }
}

void TurnDeviceOn() {
  /* Green LED on Indicates Ready State */
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, LOW);
}

void TurnDeviceOff() {
  /* RGB LED off Indicates Dormant State*/
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
}
