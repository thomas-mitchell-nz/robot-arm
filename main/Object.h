/*
 * 
 */

#ifndef OBJECT_H
#define OBJECT_H

#include "Arduino.h"

class Object {
  public:
    // Object parameters
    int size;
    int pos;
    int dist;
    int height;

    void displayObject(); // Display object properties to serial monitor
    void createObject(int start, int end, int startDist, int endDist, int height=0);
};

#endif
