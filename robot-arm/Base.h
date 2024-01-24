/*******************************************************
 * Base.h
 * Header file for Base class
 * Authors: T.J. Mitchell
 * Last modified: 21/04/22
 *******************************************************/

#ifndef BASE_H
#define BASE_H

#include <Arduino.h>

class Base {
  public:
    Base();                             // Constructor to initialize the stepper motor
    void turn(int steps);               // Turn the stepper motor clockwise (+ve steps) or counter-clockwise (-ve steps) by a specified number of steps
    int currentAngle = 0;               // Current position of the stepper motor, relative to the position at startup
};

#endif
