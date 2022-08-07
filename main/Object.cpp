/*
 * 
 */

#include "Arduino.h"
#include "Object.h"

Object::Object() {}

void Object::createObject(int start, int end, int startDist, int endDist, int height=0) {
  // Set object parameters
  size = sqrt(sq(start-end)+sq(startDist-endDist));
  pos = size/2 + start;
  this->dist = min(startDist, endDist);
  this->height = height;
}

void Object::displayObject() {
  // Display object properties to serial monitor
  Serial.println("Size: %d", size);
  Serial.println("Pos: %d", pos);
  Serial.println("Dist: %d", dist);
  Serial.println("Height: %d", height);
}
