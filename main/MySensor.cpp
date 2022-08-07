/*
 * 
 */

#include "Arduino.h"
#include "MySensor.h"

MySensor::MySensor(int echoPin, int trigPin) {
  timeout = range / speed; // set timeout acording to desired range
  this->trigPin = trigPin;
  this->echoPin = echoPin;
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
}

int MySensor::ping() {
  float distance, duration;
  
  digitalWrite(trigPin, LOW); // Set trigger to low

  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH); // Set trigger to high

  delayMicroseconds(10); // Pause while signal is emitted

  digitalWrite(trigPin, LOW); // Set trigger to low, creating pulse

  duration = pulseIn(echoPin, HIGH, timeout); // Read time taken to read pulse from reciever

  distance = duration / 2 * speed; // d = vt
  
  return distance;
}
