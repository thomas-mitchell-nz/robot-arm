/*
 * 
 */

#ifndef MYSTEPPER_H
#define MYSTEPPER_H

#include "Arduino.h"

class MyStepper {
  public:
    int pin1;
    int pin2;
    int pin3;
    int pin4;
    int stepsPerRevolution = 4096;
    
    void init(); // Initialise stepper motor
    void turnTo(int desiredPos); // Turn stepper to given position
    
  private:
    int rpm = 60; // Revolutions per minute
    int currentPos = 0; // Current position of stepper relative to position at startup
    static Stepper stepper; // Stepper object
};

#endif
