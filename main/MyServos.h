/*
 * 
 */

#ifndef MYSERVOS_H
#define MYSERVOS_H

#include "Arduino.h"
#include <Servo.h>

class MyServos
{
  public:
    // Servo pins
    int leftShoulderPin;
    int rightShoulderPin;
    int elbowPin;
    int wristPin;
    int gripperPin;

    // Default positions for arm <leftShoulder, rightShoulder, elbow, wrist>
    int retractAngles[4] = {10, 170, 170, 90};

    // Servo objects
    Servo leftShoulderServo;
    Servo rightShoulderServo;
    Servo elbowServo;
    Servo wristServo;
    Servo gripperServo;

    // Array of servos corresponding to joint movements to be iterated over
    Servo jointServos[4] = {
      leftShoulderServo,
      rightShoulderServo,
      elbowServo,
      wristServo
    };

    void attach(); // Attach servo objects to corresponding pins
    void detach(); // Detach servos
    void extendTo(int dist, int height); // Extend arm to given position
    void retract(); // Set arm to default position
    void grab(int size); // Grab object of given size
    void release(); // Release object
    void collect(int size, int dist, int height=0); // Extend to given object, grab it, and retract
    void place(int dist, int height=0); // Place object, release, and retract
    
  private:
    void rotateServos(int *angles); // Rotate joint servos to given angles
};

#endif
