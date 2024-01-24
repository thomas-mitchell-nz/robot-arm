/*******************************************************
 * Angles.h
 * Header file for Angles class
 * Authors: T.J. Mitchell
 * Last modified: 21/04/22
 *******************************************************/

#ifndef ANGLES_H
#define ANGLES_H

#include <Arduino.h>

class Angles {
  public:
    Angles(int dist, int height, bool print=false);   // Constructor to calculate angles based on given distance and height
    int shoulder, elbow, wrist;                       // Angles calculated by constructor

  private:
    const float pi = 3.14159;                         // Value of pi for calculations
    const int shaft = 20;                             // Length of arm's shaft
};

#endif