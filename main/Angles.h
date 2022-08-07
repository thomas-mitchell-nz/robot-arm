/*
 * 
 */

#ifndef ANGLES_H
#define ANGLES_H

#include "Arduino.h"

class Angles {
  public:
    // Coordinates of desired position
    int x;
    int y;

    // Angles for corresponding servos
    float shoulderAngle;
    float elbowAngle;
    float wristAngle;

    Angles(int dist, int height);
    void calculateAngles(float hyp, float elev); // Calculate angles for given position
    int *getJointAngles(); // Return angles as integers for joint movements
    void displayAngles(); // Display angles to serial monitor
    
  private:
    const int shaft = 20; // shaft length

};

#endif
