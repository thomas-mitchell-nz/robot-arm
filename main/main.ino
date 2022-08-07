/*
 * 
 */
 
// #include "Angles.h"
#include "Controller.h"
#include "Object.h"
#include "MySensor.h"
#include "MyServos.h"
#include "MyStepper.h"


//variable declarations
MySensor sensor;
Object *objects;
Object stack;
float *sample;
int sampleLength;
int numObjects;
int stackTol;

void setup () {
  // setup code, run once:
  stepper.init();
  servos.attach();

  stackTol = 5;
  sampleLength = stepper.stepsPerRevolution;
}

void loop () {
  // main code, runs repeatedly:
  sweep(sample, sampleLength);
  
  objects = detectObjects(sample, sampleLength, &numObjects, false);
  sortBySize(objects, numObjects);

  stack.height = objects[0].height;
  stack.pos = objects[0].pos;

  for (uint8_t i = 0; i < numObjects; i++) {
    collectAndPlace(objects[i]);
    stack.height += objects[i].size;
  }

  exit(0);
}

void Controller::sweep(int *sample, n) {
  sample = calloc(n, sizeof(int));

  for (int i = 0; i < n; i++) {
    sample[i] = sensor.ping()
    stepper.step(1);
  }
}

void sortBySize(int* sample, int n) {
  // Sort elements in sample by size in descending order
  int i, j, tmp;
  
  for (i = 0; i < n; i++) {
    for (j = i+1; j < n; j++) {
      if (sample[i].size < sample[j].size) {
        tmp = sample[i];
        sample[i] = sample[j];
        sample[j] = tmp;
      }
    }
  }
}

void collectAndPlace(MyObject object) {
  // Collects and places object on stack
  stepper.turnTo(object.pos);
  servos.collect(object.size, sensor.ping());
  stepper.turnTo(stack.pos);
  servos.place(sensor.ping(), stack.height + stackTol);
}
