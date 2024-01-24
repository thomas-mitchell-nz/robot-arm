/*******************************************************
 * global.h
 * Header file for global variables and objects
 * Authors: T.J. Mitchell
 * Last modified: 17/10/22 
 *******************************************************/

#ifndef GLOBAL_H_
#define GLOBAL_H_

// Include necessary libraries and headers
#include <Stepper.h>
#include <Servo.h>

// Define macros
#define PRINT_TARGET  false
#define PRINT_SONAR   false
#define PRINT_ANGLES  false

// Declare global variables
extern const int stepsPerRev;

// Declare global objects
extern Stepper stepper;

#endif
