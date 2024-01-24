/*******************************************************
 * Ultrasonic.cpp
 * Ultrasonic class source file to poll ultrasonic sensors
 * Authors: T.J. Mitchell
 * Last modified: 17/10/22
 *******************************************************/

#include "Ultrasonic.h"
#include "global.h"

// Constructor
Ultrasonic::Ultrasonic(int tPin, int ePin, bool _print) {
  trigPin = tPin;                 // Set trigPin to given parameter
  echoPin = ePin;                 // Set echoPin to given parameter
  pinMode(trigPin, OUTPUT);       // Set trigPin to output mode
  pinMode(echoPin, INPUT);        // Set echoPin to input mode
  print = _print;                 // Set the print flag to given parameter
}

 // Returns range in cm
float Ultrasonic::getRange() {
  digitalWrite(trigPin, HIGH);    // Sends signal through trigPin
  delayMicroseconds(10);          // Wait for 10 µs while signal is emitted
  digitalWrite(trigPin, LOW);     // Stop sending signal through trigPin

  
  time = pulseIn(echoPin, HIGH, timeout);  // Read duration of pulse from echoPin and calculate the range
  range = (time * vel) / 2;       // d = vt where v = 0.0343 cm/µs

  // Print the range if the print flag is true
  if (print) {
    Serial.print("Range (cm):\t"); 
    Serial.println(range);
  }

  return range; // Return the calculated range
}
