/*******************************************************
 * Arm.h
 * Header file for Arm class
 * Authors: T.J. Mitchell
 * Last modified: 21/04/22
 *******************************************************/

#ifndef ARM_H
#define ARM_H

#include <Arduino.h>
#include <Servo.h>

class Arm
{
  public:
    Arm(int ccwPin, int cwPin, int ePin, int wPin);
    void attachServos();                    // Attaches servo objects to corresponding pins
    void detachServos();                    // Detaches servos
    void retract();                         // Moves the arm to the resting position
    void extendTo(int dist, int height=0);  // Extends the arm to a specified distance and height

  private:
    // Resting position angles
    const int shoulderRest = 10;
    const int elbowRest = 170;
    const int wristRest = 90;

    // Servo objects
    Servo ccwShoulderServo;
    Servo cwShoulderServo;
    Servo elbowServo;
    Servo wristServo;

    // Pin variables
    int ccwShoulderPin;
    int cwShoulderPin;
    int elbowPin;
    int wristPin;
};

#endif