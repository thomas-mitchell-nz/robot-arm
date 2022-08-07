/*
 * 
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Arduino.h"

class Angles {
  public:
    int maxObject = 10; // Max number of objects to be detected
    
    void *detectObjects(int *sample, int n, int *num, bool printData); // Detects objects in given sample
    
    void displayObjects(Object *objects, int n); // Lists the objects in tidy rows and columns
    
  private:
    void sortDescending(int *array, int n); // Sort elements of an array in descending order

    int getDist(int *dists, int start, int end); // Return smallest distance in range
    
    int countFreq(int *sample, int *values, int *freq, int n); // Count frequency of elements in sample
    
    int *cleanData(int *sample, int n); // Remove noise from sample
    
    float *getKPointAverages(int *distVals, int backgroundDist, int k, int n2); // Returns a list of the k point averages of the given data
    
    float normalise(int num, int n); // Normalise num to 360 degrees 
    
    int getMax(int *array, int n); // Returns max value in array
};

#endif
