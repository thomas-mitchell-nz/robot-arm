/*******************************************************
 * Ultrasonic.h
 * Ultrasonic class header file
 * Authors: T.J. Mitchell
 * Last modified: 17/10/22
 *******************************************************/

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#include <Arduino.h>

class Ultrasonic {
  public:
    // Constructor that takes an optional boolean argument for printing output
    Ultrasonic(int tPin, int ePin, bool _print=false);

    // Returns the range of the ultrasonic sensor in centimeters
    float getRange();
    
  private:
    int trigPin, echoPin;
    int time, range;            // Variables for holding time and range
    const float vel = 0.0343;   // Speed of sound in air (cm/µs)
    const unsigned long timeout = 6000;
    bool print;                 // Boolean flag for printing output
};

#endif

