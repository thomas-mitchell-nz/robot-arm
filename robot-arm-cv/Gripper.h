/*******************************************************
 * Gripper.h
 * Header file for Gripper class
 * Authors: T.J. Mitchell
 * Last modified: 21/04/22
 *******************************************************/

#ifndef GRIPPER_H
#define GRIPPER_H

#include <Servo.h>
#include <Arduino.h>

class Gripper {
  public:
    // Constructor
    Gripper(int gPin);

    // Attach the gripper servo to the corresponding pin
    void attach();

    // Close the gripper with the given size
    void grab(int size);

    // Open the gripper
    void release();

  private:
    Servo gripperServo; // Servo object for the gripper
    int gripperPin;     // Pin variable
};

#endif