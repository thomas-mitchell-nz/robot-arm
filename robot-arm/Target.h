/*******************************************************
 * Target.h
 * Target class header file for target object parameters
 * Authors: T.J. Mitchell
 * Last modified: 12/04/23
 *******************************************************/

#ifndef TARGET_H_
#define TARGET_H_

#include <Arduino.h>
#include <math.h>

class Target {
public:
    Target(int startAngle=0, int endAngle=0, int startDist=0, int endDist=0, bool print = false, int _height=0);
    float size;    // Size of the target
    float pos;     // Position of the target
    int dist;      // Distance to the target
    int height;    // Height of the target
};

#endif