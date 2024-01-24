/*******************************************************
 * Arm.cpp
 * Controls the movement of the arm's servos
 * Authors: T.J. Mitchell
 * Last modified: 21/04/22
 *******************************************************/

#include "Arm.h"
#include "global.h"
#include "Angles.h"
#include <Servo.h>

// Constructor
Arm::Arm(int ccwPin, int cwPin, int ePin, int wPin) {
  ccwShoulderPin = ccwPin;
  cwShoulderPin = cwPin;
  elbowPin = ePin;
  wristPin = wPin;
}

// Attach all servo objects to their corresponding pins
void Arm::attachServos() {
  cwShoulderServo.attach(cwShoulderPin);
  ccwShoulderServo.attach(ccwShoulderPin);
  elbowServo.attach(elbowPin);
  wristServo.attach(wristPin);
}

// Detach all servo objects
void Arm::detachServos() {
  cwShoulderServo.detach();
  ccwShoulderServo.detach();
  elbowServo.detach();
  wristServo.detach();
}

// Retract the arm to the default position
void Arm::retract() {
  cwShoulderServo.write(shoulderRest);
  ccwShoulderServo.write(180-shoulderRest);
  elbowServo.write(elbowRest);
  wristServo.write(wristRest);
  delay(2000);
}

// Extend the arm to a given distance and height
void Arm::extendTo(int dist, int height) {
  Angles angles(dist, height, PRINT_ANGLES);    // Calculate the angles

  cwShoulderServo.write(angles.shoulder);
  ccwShoulderServo.write(180-angles.shoulder);
  elbowServo.write(angles.elbow);
  wristServo.write(angles.wrist);

  delay(2000); 
}