#include <omp.h>
#include <string>
#include <iostream>

#ifndef MERGESORT_H
#define MERGESORT_H

void mergeSortSeq(int arr[], int tmp[], int n);
void mergeSortPar(int arr[], int tmp[], int n);
void mergeSortRecSeq(int arr[], int l, int r);
void mergeSortRecPar(int arr[], int l, int r);
bool isOrdered(int array[], int arraySize);

#endif //MERGESORT_H
