#include <omp.h>
#include <string>
#include <iostream>

#ifndef MERGESORT_H
#define MERGESORT_H

void mergeSortSeq(int arr[], int n);
void mergeSortPar(int arr[], int n);
bool isOrdered(int array[], int arraySize);

#endif //MERGESORT_H
