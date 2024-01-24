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
    Target(int _idx=0, int _pos=0, int _dist=0, bool _print=0);
    float idx;    // Target index
    float pos;    // Position of the target
    int dist;     // Distance to the target
    int print;    // Print flag
};

#endif