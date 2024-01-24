/*******************************************************
 * Gripper.cpp
 * Controls gripper
 * Authors: T.J. Mitchell
 * Last modified: 21/04/22
 *******************************************************/

#include "Gripper.h"
#include "global.h"
#include <Servo.h>

// Constructor
Gripper::Gripper(int gPin) {
  gripperPin = gPin;
}

// Attach the gripper servo to the corresponding pin
void Gripper::attach() {
  gripperServo.attach(gripperPin);
}

// Close the gripper with the given size
void Gripper::grab(int size) {
  gripperServo.write(size);
  delay(500); 
}

// Open the gripper
void Gripper::release() {
  gripperServo.write(0);
  delay(500); 
}