/*******************************************************
 * Target.cpp
 * Target class source file for target object parameters
 * Authors: T.J. Mitchell
 * Last modified: 12/04/23
 *******************************************************/

#include "Target.h"

Target::Target(int startAngle, int endAngle, int startDist, int endDist, bool print, int _height) {
    // Calculate the size of the target
    size = abs(startAngle-endAngle); // 2*sin(abs(startAngle-endAngle)*3.14/1024)*startDist;
    
    // Calculate the position of the target
    pos = abs(startAngle-endAngle) / 2 + startAngle;

    // Calculate the distance to the target
    dist = min(startDist, endDist);

    // Assign the height of the target
    height = _height;

    // Print target details if required
    if (print) {
        Serial.println("*******************************");
        Serial.print("Size:\t"); Serial.println(size);
        Serial.print("Pos:\t"); Serial.println(pos);
        Serial.print("Dist:\t"); Serial.println(dist);
        Serial.print("Height:\t"); Serial.println(height);
        Serial.println("*******************************");
    }
}