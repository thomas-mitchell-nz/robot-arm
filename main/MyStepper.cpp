/*
 * 
 */

#include "Arduino.h"
#include "MyStepper.h"

void MyStepper::MyStepper(int pin1, int pin2, int pin3, int pin4) {
  // Set corresponding pins
  this->pin1 = pin1;
  this->pin2 = pin2;
  this->pin3 = pin3;
  this->pin4 = pin4;
}

void MyStepper::init() {
  // Initialise stepper motor
  stepper(stepsPerRevolution, pin1, pin3, pin2, pin4);
  setSpeed(rpm);
}

void MyStepper::turnTo(int desiredPos) {
  // Turn stepper to given position
  int steps = currentPos - desiredPos;
  stepper.step(steps);
  currentPos = desiredPos;
}
