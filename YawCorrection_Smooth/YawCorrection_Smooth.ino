#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 1);

int ndat=5;      // Number of data points to smooth
float avgDeg;    // Used for smoothing data, average of incoming data
int cntDat;      // Number of data points that have passed since last average

void setup() {
  Serial.begin(38400);
  AFMS.begin();
  myMotor->setSpeed(50);
  cntDat = 0;    //Initialize data count
  avgDeg = 0;    //Initialize average degree
}

void loop() {

  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte and add to average count
    avgDeg+=Serial.parseFloat() / ndat;
    cntDat++;        // Iterate count of data points
    
      if (avgDeg > 0 && cntDat >= ndat-1) {
        myMotor->step(avgDeg * 1.425, FORWARD, SINGLE);
        Serial.print("Yaw: ");
        Serial.println(avgDeg, DEC);
        serialFlush(); 
        avgDeg = 0;          //Reset average degree
        cntDat = 0;          //Reset data count
      }
      if (avgDeg < 0 && cntDat >= ndat-1) {
        myMotor->step(abs(avgDeg) * 1.425, BACKWARD, SINGLE);
        Serial.print("Yaw: ");
        Serial.println(avgDeg, DEC);
        serialFlush();
        avgDeg = 0;          //Reset average count
        cntDat = 0;          //Reset data count
      }

  }
}


//This function removes all data from serial buffer that
//was received while motor was moving.
void serialFlush() {
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}

