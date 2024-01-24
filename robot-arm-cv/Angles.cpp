/*******************************************************
 * Angles.cpp
 * Calculates the angles needed to position the arm 
 * Authors: T.J. Mitchell
 * Last modified: 21/04/22
 *******************************************************/

#include "Angles.h"
#include "math.h"

Angles::Angles(int dist, int height, bool print) {
  // Coordinates of desired position
  float x = dist;
  float y = height;

  // Triangle parameters 
  float hyp = (float) sqrt(sq(x)+sq(y));
  float elev = (float) atan2(y,x) * 180/pi;
  
  // Calculate angles based on arm geometry
  shoulder = (float) acos(hyp / 2 / shaft) * 180/pi;
  elbow = (float) 2 * asin(hyp / 2 / shaft) * 180/pi;
  wrist = shoulder - elev;
  shoulder += elev;

  // Print angles to Serial Monitor
  if (print) {
    Serial.println("*******************************");
    Serial.print("Shoulder:\t"); Serial.println(shoulder);
    Serial.print("Elbow:\t\t"); Serial.println(elbow);
    Serial.print("Wrist:\t\t"); Serial.println(wrist);
    Serial.print("Elev:\t\t"); Serial.println(elev);
    Serial.println("*******************************");
  }

  // Map angles to servo positions
  shoulder = 130 - shoulder;
  elbow = (float) -19/20 * elbow + 220;
  wrist += 90;
}