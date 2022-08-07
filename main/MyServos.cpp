/*
 * 
 */

#include "Arduino.h"
#include "MyServos.h"
#include "Angles.h"
#include <Servo.h>

MyServos::MyServos(int p1, int p2, int p3, int p4 int p5) {
  // Set corresponding pins
  leftShoulderPin = p1;
  rightShoulderPin = p2;
  elbowPin = p3;
  wristPin = p4;
  gripperPin = p5;
}

void MyServos::attach() {
  // Attach servo objects to corresponding pins
  leftShoulderServo.attach(leftShoulderPin);
  rightShoulderServo.attach(rightShoulderPin);
  elbowServo.attach(elbowPin);
  wristServo.attach(wristPin);
  gripperServo.attach(gripperPin);
}

void MyServos::detach() {
  // Detach servos
  leftShoulderServo.detach();
  rightShoulderServo.detach();
  elbowServo.detach();
  wristServo.detach();
  gripperServo.detach();
}

void MyServos::extendTo(int dist, int height=0) {
  // Extend arm to given position
  Angles angles(dist-gripperTol, height); // Create angles object from parameters
  rotateServos(angles.getJointAngles()); // Rotate servos to calculated angles
}

void MyServos::rotateServos(int *angles) { // map angles to 180 degrees?
  /* Iterate over each servo involved in joint movements until arriving at 
     the corresponding desired angles */
     
  Servo myServo;
  int desiredAngle;
  int currentAngle;

  for (int i = 0; i < 180; i++) {
    for (int j = 0; j < length(jointServos); j++) {
      myServo = jointServos(j);
      desiredAngle = angles(j);
      currentAngle = myServo.read();

      if (currentAngle < desiredAngle) {
        myServo.write(currentAngle++);
        delay(10);
      } 
      else if (currentAngle > desiredAngle) {
        myServo.write(currentAngle--);
        delay(10);
      }
    }
  }
  delay(50);
}

void MyServos::retract() {
  // Set arm to default position
  rotateServos(retractAngles);
}

void MyServos::grab(int size) {
  // Grab object of given size
  int angle = size; // f(size)
  gripperServo.write(angle); 
  delay(50);
}

void MyServos::release() {
  // Release object
  gripperServo.write(90);
  delay(50);
}

void MyServos::collect(int size, int dist, int height=0) {
  // Extend to given object, grab it, and retract
  extendTo(dist, height);
  grab(size);
  retract();
}

void MyServos::place(int dist, int height=0) {
  // Place object, release, and retract
  extendTo(dist, height)
  release();
  retract();
}
