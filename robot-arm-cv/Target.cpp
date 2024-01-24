/*******************************************************
 * Target.cpp
 * Target class source file for target object parameters
 * Authors: T.J. Mitchell
 * Last modified: 12/04/23
 *******************************************************/

#include "Target.h"

Target::Target(int _idx, int _pos, int _dist, bool _print) {
    // Assign variables
    idx = _idx;
    pos = _pos;
    dist = _dist;
    print = _print;

    // Print target details if required
    if (print) {
        Serial.println("*******************************");
        Serial.print("Idx:\t"); Serial.println(idx);
        Serial.print("Pos:\t"); Serial.println(pos);
        Serial.print("Dist:\t"); Serial.println(dist);
        Serial.println("*******************************");
    }
}