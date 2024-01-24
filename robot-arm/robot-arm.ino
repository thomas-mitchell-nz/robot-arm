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
const int maxStep = 1024;
const int minDist = 22;
const int maxDist = 40;

int startAngle, endAngle;
int startDist, endDist;
int stackHeight = 0;
int numTargets = 0;

bool obstruction = false;

// Create objects
Target targets[3] = { Target(), Target(), Target() };
Stepper stepper(stepsPerRev, stepPin1, stepPin3, stepPin2, stepPin4);
Arm arm(ccwShoulderPin, cwShoulderPin, elbowPin, wristPin);
Ultrasonic sonar(trigPin, echoPin, PRINT_SONAR);
Gripper gripper(gripperPin);
Base base;

// Setup function
void setup() {
  // Start serial communication
  Serial.begin(9600);
  
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
  int dist = sonar.getRange();                        // Get distance from sonar

  if (dist >= minDist && dist <= maxDist) {           // If obstruction within range
    if (!obstruction) {                               // If obsutruction not already detected
      obstruction = true;
      startAngle = base.currentAngle;
      startDist = dist;
    }
    endAngle = base.currentAngle;
    endDist = dist;
  } 
  else {                                               
    if (obstruction) {                                // Just passed the trailing edge of the obstruction
      obstruction = false;
      targets[numTargets] = Target( startAngle, endAngle, startDist, endDist, PRINT_TARGET );
      numTargets++;                                     
    }
  }

  if (base.currentAngle >= maxStep) {
    sortTargets();                                    // Sort targets by decreasing size
    stackHeight += 3;                                 // Record the stack height

    for (int i = 1; i < numTargets; i++) {            // Iterate through all identified targets
      stackHeight += 3;                               // Update stack height
      base.turn(targets[i].pos-base.currentAngle-10); // Turn to the target object
      arm.extendTo(sonar.getRange()-7, -2);           // Extend arm to target (minus tolerance)
      gripper.grab(80);                               // Close gripper 
      arm.retract();                                  // Retract arm                            
      base.turn(targets[0].pos - base.currentAngle);  // Turn to the largest object
      arm.extendTo(sonar.getRange()-8, stackHeight);  // Extend arm to above the stack
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
      if (targets[i].size < targets[j].size && targets[i].size != 0) {
        tmp = targets[i];
        targets[i] = targets[j];
        targets[j] = tmp;
      }
    }
  }

  if (PRINT_TARGET) {
    for (int i = 0; i < numTargets; i++) { 
      Serial.println("*******************************");
      Serial.print("Target:\t"); Serial.println(i);
      Serial.print("Size:\t"); Serial.println(targets[i].size);
      Serial.print("Pos:\t"); Serial.println(targets[i].pos);
      Serial.print("Dist:\t"); Serial.println(targets[i].dist);
      Serial.print("Height:\t"); Serial.println(targets[i].height);
      Serial.println("*******************************");
    }
  }
}