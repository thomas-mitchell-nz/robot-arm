/*
 * 
 */

#include "Arduino.h"
#include "Angles.h"

Angles::Angles(int dist, int height) {
  // Set parameters 
  x = dist, y = height;
  float hyp = (float) sqrt(sq(x)+sq(y));
  float elevation = (float) atan(y/x);
  calculateAngles(hyp, elevation);
}

void Angles::calculateAngles(float hyp, float elev) {
  // Calculate angles based on arm geometry
  shoulderAngle = acos(hyp / 2 / shaft);
  elbowAngle = 2 * asin(hyp / 2 /shaft);
  wristAngle = shoulderAngle - elev;

  // Map angles to servo positions
  shoulderAngle += 130;
  elbowAngle = -19/20 * elbowAngle + 220;
  wristAngle += 90;
}

int Angles::*getJointAngles() {
  // Return array of angles for joint movements 
  int angles[4] = {
    shoulderAngle, 
    180-shoulderAngle, // Right shoulder servo rotates opposite direction to left
    elbowAngle, 
    wristAngle
  };
    
  return angles;
}

void Angles::displayAngles() {
  // Display angles to serial monitor
  Serial.println("Given dist = %d and height = %d:", x, y);
  Serial.println("- Shoulder Angle: %d", shoulderAngle);
  Serial.println("- Elbow Angle: %d", elbow);
  Serial.println("- Wrist Angle: %d", wrist);
}
