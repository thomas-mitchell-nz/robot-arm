/*******************************************************
 * Base.cpp
 * Controls the movement of a stepper motor that
 * Authors: T.J. Mitchell
 * Last modified: 21/04/22
 *******************************************************/

#include "global.h"
#include <Stepper.h>
#include "Base.h"

// Constructor
Base::Base() {}

// Turn stepper to desired number of steps clockwise
void Base::turn(int steps) {
  int j = steps/abs(steps);

  for (int i = 0; i < abs(steps); i++) {
    stepper.step(j);                            // Step once clockwise
    currentAngle += j;                          // Update the current position
    currentAngle = currentAngle % stepsPerRev;  // Map the current position to a value between 0 and 4095
    delay(5);
  }
}
