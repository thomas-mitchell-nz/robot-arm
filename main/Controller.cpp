/*
DetectObjects.c

Created by Thomas Mitchell and Ben Ross on 02.01.2022

This file contains a system for detecting objects based 
on an array of distance values in a circle. 
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "Arduino.h"
#include "Controller.h"
#include "Object.h"

void Controller::Controller() {}

void Controller::sortDescending(int *array, int n) {
  // Sort elements of an array in descending order
  int i, j, tmp;
  
  for (i = 0; i < n; i++) {
    for (j = i+1; j < n; j++) {
      if (array[i] < array[j]) {
        tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;
      }
    }
  }
}

int Controller::getDist(int *dists, int start, int end) {
  // Return smallest distance in range
  int tmp = dists[0];
  for (int i = start; i < end; i++) {
    if (dists[i] < tmp) {
      tmp = dists[i];
    }
  }
  return tmp;
}

int Controller::countFreq(int *sample, int *values, int *freq, int n) {
  // Count frequency of elements in sample
  int i, j, count, numUnique=0;
   
  for(i = 0; i < n; i++) {
    // Copy elements from data to values
    values[i] = sample[i];
        // Initially initialize frequencies to -1
        freq[i] = -1;
    }
  
  // Sort elements of an array in descending order
  sortDescending(values, n);
   
  // Traverse through array elements and count frequencies
    for (i = 0; i < n; i++) {
        count = 1;
        for (j = i+1; j < n; j++)
        {
            // If duplicate element is found
            if(values[i] == values[j]) {
        values[i] = values[j];
                count++;

                // Make sure not to count frequency of same element again
                freq[j] = 0;
            }
        }

        // If frequency of current element is not counted
        if (freq[i] != 0) {
            freq[i] = count;
        }
  }
  
  // Assign unique elements to beginning of arrays
  for (i = 0; i < n; i++) {
    if (freq[i] != 0 && values[i] != 0) {
      values[numUnique] = values[i];
      freq[numUnique] = freq[i];
      numUnique++;
    }
  }
  
  return numUnique;
}

int Controller::*cleanData(int *sample, int n) {
  // Remove noise from sample
  int *cleanData = calloc(n, sizeof(int));
  int *values = calloc(n, sizeof(int));
  int *freq = calloc(n, sizeof(int));
  int maxValue, numUnique;
  
  // Get frequency of sample elements
  numUnique = countFreq(sample, values, freq, n);

  // Find background value
  for (int i = 0; i < numUnique; i++) {
    // If frequent enough to be a true background value
    if ((float) freq[i]/n > 0.1) { 
      maxValue = values[i];
      break;
    }
  }
  
  // Remove noise from sample by overwriting noise with backgroud value
  for (int i = 0; i < n; i++) {
    if (maxValue < sample[i]) {
      cleanData[i] = maxValue;
    } else {
      cleanData[i] = sample[i];
    }
  }
  
  /* Free dynamically allocated arrays from memory */
  free(values);
  free(freq);
  
  return cleanData;
}

float Controller::*getKPointAverages(int *distVals, int backgroundDist, int k, int n2) {
  // Returns a list of the k point averages of the given data
  int n1 = k-1;
  int n3 = n1 + n2;
  int sum=0, pos=0;

  float *kPointAverages = calloc(n3,sizeof(int));
  int *paddedDistVals = calloc(n3, sizeof(int));
  int *arrayNumbers = calloc(k, sizeof(int));
  
  // Pad data with background values to allow for trailing k point average
  for (int i = 0; i < n1; i++) {
    paddedDistVals[i] = backgroundDist;
  }
  
  // Copy elements from distVals to paddedDistVals
  for (int i = 0, j = n1; j < n3; i++, j++) {
    paddedDistVals[j] = distVals[i];
  }
  
  // Calculate moving average
  for (int i = 0; i < n3; i++) {
    // Subtract the oldest number from the sum, add the new number
    sum = sum - arrayNumbers[pos] + paddedDistVals[i];
    // Assign the nextNum to the position in the array
    arrayNumbers[pos] = paddedDistVals[i];
    
    if (i < k) {
      // First k avergaes
      kPointAverages[i] = (float) sum/(i+1);
    } else {
      kPointAverages[i] = (float) sum/k;
    }
    
    pos++;
    if (pos >= k) {
      pos = 0;
    }
  }
  
  // Free arrays from memory
  free(paddedDistVals);
  free(arrayNumbers);
  
  return kPointAverages;
}

void Controller::displayObjects(Object *objects, int n) {
  // Lists the objects in tidy rows and columns
  float start, end, width;
  
  printf("Index     Start       End    Width\n");

  for (int i = 0; i < n; i++) {
    Object obj = objects[i];
    start = obj.start;
    end = obj.end;
    width = obj.width;
    printf("%d %13.3f %9.3f %8.3f\n", i, start, end, width);
  }
}

float Controller::normalise(int num, int n) {
  // Normalise num to 360 degrees 
  return (float) num * 360 / n;
}

int getMax(int *array, int n) {
  // Returns max value in array
  int tmp = array[0];
  for (int i = 1; i < n; i++) {
    if (array[i] > tmp) {
      tmp = array[i];
    }
  }
  return tmp;
}

void Controller::*detectObjects(int *sample, int n, int *num, bool printData) {
  /* Takes an array of distance values and detects significance
    changes in distances to signify objects */ 
  int *cleanDists, backgroundDist, numAverages, idx, startIndex=0, k=5; // Arbitrary
  float *kPointAverages, previousAverage, percentChange, percentChangeThreshold=0.05;
  bool recordingObject = false;
  
  Object *objects = calloc(maxObjects, sizeof(Object));

  cleanDists = cleanData(sample, n); // Remove noise from sample
  
  backgroundDist = getMax(cleanDists, n); // Find true distance value
  
  kPointAverages = getKPointAverages(cleanDists, backgroundDist, k, n); // Get moving averages
  
  numAverages = n+k-1; // Number of averages due to additional padded data
  
  previousAverage = kPointAverages[0];
  
  recordingObject = (previousAverage-backgroundDist)/backgroundDist<percentChangeThreshold; // If object is being detected 
  
  for (idx = 0; idx < numAverages; idx++) {
  
    percentChange = (kPointAverages[idx]-previousAverage)/previousAverage;  // Degree of change between previous average
    
    if (percentChange < -percentChangeThreshold && !recordingObject) { // If recording object
      startIndex = idx;
      recordingObject = true;
    } 
    else if (percentChange > percentChangeThreshold && recordingObject) { // If trailing edge of object detected
      (object+*num).createObject(startIndex, idx-1, cleanDists[startIndex], cleanDists[idx-1]);
      recordingObject = false;
      (*num)++;
    }
      
    if (printData) {
      // Print data to serial monitor
      Serial.print("Idx: %d, ", idx);
      Serial.print("Dist: %d, ", cleanDists[idx]);
      Serial.print("%%Change: %f, ", percentChange);
      Serial.println("Detecting: %s", recordingObject ? "true" : "false");
    }
    
    previousAverage=kPointAverages[idx];
  }
  
  if (recordingObject) { // Add last object if it's cutoff
    (object+*num).createObject(startIndex, idx-1, cleanDists[startIndex], cleanDists[idx-1]);
  }
 
  free(kPointAverages); // Free array from memeory
  
  return objects; 
}
