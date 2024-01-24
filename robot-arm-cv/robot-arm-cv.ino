/*******************************************************
 * main.ino
 * Main project file
 * Authors: T.J. Mitchell
 * Last modified: 21/04/22
 *******************************************************/

// Include required libraries
#include <Arduino.h> 
#include <Stepper.h>
#include "Target.h"
#include "Ultrasonic.h"
#include "Arm.h"
#include "Base.h"
#include "Gripper.h"
#include "Angles.h"
#include "global.h"

// Define pins
int trigPin = 15;
int echoPin = 14;

int elbowPin = 4;
int wristPin = 5;
int gripperPin = 6;
int cwShoulderPin = 3;
int ccwShoulderPin = 2;

int stepPin1 = 9;
int stepPin2 = 10;
int stepPin3 = 11;
int stepPin4 = 12; 

// Declare variables
const int stepsPerRev = 4096;
const int maxStep = 700;
const int minDist = 22;
const int maxDist = 40;
int stackHeight = 0;
int numTargets = 0;
String str = "";
int itr = 0;
char *token;
int msg[2];
int dist;

// Create objects
Target targets[2] = { Target(), Target() };
Stepper stepper(stepsPerRev, stepPin1, stepPin3, stepPin2, stepPin4);
Arm arm(ccwShoulderPin, cwShoulderPin, elbowPin, wristPin);
Ultrasonic sonar(trigPin, echoPin, PRINT_SONAR);
Gripper gripper(gripperPin);
Base base;

// Setup function
void setup() {
  // Start serial communication
  Serial.begin(115200);
  Serial.setTimeout(100);
  
  // Attach servos and retract arm
  arm.attachServos();
  arm.retract();

  // Attach and release gripper
  gripper.attach();
  gripper.release();

  // Wait for 2 seconds
  delay(2000);
}

// Loop function
void loop() {
  base.turn(1);                                       // Step clockwise
  delay(30);                                          // Pause for 30 ms

  if (Serial.available()) {                           // Serial data available
    str = Serial.readString();                        // Unpack serial data
 
    if (str != "") {                                  // If str not empty
      token = strtok(str.c_str(), " ");               // Split string with " " delimiter
      while (token != NULL) {                         // For each sub-string in string
        msg[itr] = atoi(token);                       // Store token as integer 
        token = strtok(NULL, " ");                    // Get the next token
        itr++;
      }   

      dist = int(msg[1]/10);                          // Convert dist from mm to cm

      if (dist >= minDist && dist <= maxDist) {       // If target within range
        targets[numTargets] = Target( msg[0], base.currentAngle, dist, PRINT_TARGET );
        numTargets++;
      }

      // Clear variables
      str = "";
      dist = 0;
      itr = 0;
    }
  }

  if (base.currentAngle >= maxStep) {
    sortTargets();                                    // Sort targets by decreasing size
    stackHeight += 4;                                 // Record the stack height

    for (int i = 1; i < numTargets; i++) {            // Iterate through all identified targets
      stackHeight += 3;                               // Update stack height
      base.turn(targets[i].pos-base.currentAngle-10);    // Turn to the target object
      arm.extendTo(targets[i].dist-7);            // Extend arm to target (minus tolerance)
      gripper.grab(80);                               // Close gripper 
      arm.retract();                                  // Retract arm                            
      base.turn(targets[0].pos - base.currentAngle);  // Turn to the largest object
      arm.extendTo(targets[i].dist-13, stackHeight);   // Extend arm to above the stack
      gripper.release();                              // Drop the object onto the stack
      arm.retract();                                  // Retract arm
    }

    exit(0);                                          // Terminate the program
  }
}

// Sort array of targets by decreasing object size
void sortTargets() {
  Target tmp;
  int i, j;
  
  for (i = 0; i < numTargets; i++) {
    for (j = i+1; j < numTargets; j++) {
      if (targets[i].idx > targets[j].idx) {
        tmp = targets[i];
        targets[i] = targets[j];
        targets[j] = tmp;
      }
    }
  }

  if (PRINT_TARGET) {
    for (int i = 0; i < numTargets; i++) { 
      Serial.println("*******************************");
      Serial.print("Idx:\t"); Serial.println(targets[i].idx);
      Serial.print("Pos:\t"); Serial.println(targets[i].pos);
      Serial.print("Dist:\t"); Serial.println(targets[i].dist);
      Serial.println("*******************************");
    }
  }
}