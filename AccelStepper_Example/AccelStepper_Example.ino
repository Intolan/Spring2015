// Shows how to run three Steppers at once with varying speeds
//
// Requires the Adafruit_Motorshield v2 library 
//   https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library
// And AccelStepper with AFMotor support 
//   https://github.com/adafruit/AccelStepper

// This tutorial is for Adafruit Motorshield v2 only!
// Will not work with v1 shields

#include <AccelStepper.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

Adafruit_MotorShield AFMStop(0x60); // Default address, no jumpers

// Connect two steppers with 200 steps per revolution (1.8 degree)
// to the top shield
Adafruit_StepperMotor *myStepper = AFMStop.getStepper(513, 1);



// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
// wrappers for the first motor!
void forwardstep() {  
  myStepper->onestep(FORWARD, SINGLE);
}
void backwardstep() {  
  myStepper->onestep(BACKWARD, SINGLE);
}


// Now we'll wrap the 3 steppers in an AccelStepper object
AccelStepper stepper(forwardstep, backwardstep);
int cnt;

void setup()
{  
  AFMStop.begin(); // Start the top shield
  Serial.begin(9600);
  stepper.setMaxSpeed(50.0);
  stepper.setAcceleration(100.0);
  //stepper.moveTo(257);
  cnt=0;
}

void loop()
{
    // Change direction at the limits
    if (Serial.available() > 0) {
      int dist = Serial.parseInt();
      if (dist != 0 ) {
        stepper.moveTo(stepper.currentPosition()+dist);
        Serial.println(dist);
        serialFlush();
      }
    }
    
    stepper.run();
      Serial.println(stepper.currentPosition());
    
}

void serialFlush() {
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}

