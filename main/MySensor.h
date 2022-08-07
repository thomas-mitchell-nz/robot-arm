/*
 * 
 */

#ifndef MYSENSOR_H
#define MYSENSOR_H

#include "Arduino.h"

class MySensor {
  public:
    int ping(); // Return distance from sensor
    
  private:
    float range = 40; // Max distance
    float speed = 0.0343; // v = 0.0343 cm/us
    int timeout;
    int trigPin;
    int echoPin;

};

#endif
