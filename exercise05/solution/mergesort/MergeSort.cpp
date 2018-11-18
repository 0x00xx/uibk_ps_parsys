#include "MergeSort.h"
#include <cstring>

using namespace std;
void merge(int A[], int temp[], int from, int mid, int to, int n);
void mergePar(int A[], int temp[], int from, int mid, int to, int n);

int min(int x, int y) {
	return (x < y) ? x : y;
}

void mergeSortSeq(int arr[], int temp[], int n) {
    // Merge subarrays in bottom up manner.  First merge subarrays of 
    // size 1 to create sorted subarrays of size 2, then merge subarrays 
    // of size 2 to create sorted subarrays of size 4, and so on. 
    for (int curr_size=1; curr_size<=n-1; curr_size = 2*curr_size) { 
        // Pick starting point of different subarrays of current size 
        for (int left_start=0; left_start<n-1; left_start += 2*curr_size) { 
            // Find ending point of left subarray. mid+1 is starting  
            // point of right 
            int mid = left_start + curr_size - 1; 
            int right_end = min(left_start + 2*curr_size - 1, n-1); 
  
            // Merge Subarrays arr[left_start...mid] & arr[mid+1...right_end] 
            merge(arr, temp, left_start, mid, right_end, n); 
        } 
    }
}

void merge(int A[], int temp[], int from, int mid, int to, int n) {
	int k = from, i = from, j = mid + 1;

	// loop till there are elements in the left and right runs
	while (i <= mid && j <= to) {
		if (A[i] < A[j])
			temp[k++] = A[i++];
		else
			temp[k++] = A[j++];
	}

	// Copy remaining elements
	while (i < n && i <= mid)
		temp[k++] = A[i++];

	// Don't need to copy second half

    
	// copy back to the original array to reflect sorted order
    memcpy(&A[from], &temp[from], (to - from + 1) * sizeof(int));
}

void mergePar(int A[], int temp[], int from, int mid, int to, int n) {
	int k = from, i = from, j = mid + 1;

	while (i <= mid && j <= to) {
		if (A[i] < A[j])
			temp[k++] = A[i++];
		else
			temp[k++] = A[j++];
	}

	while (i < n && i <= mid)
		temp[k++] = A[i++];

    memcpy(&A[from], &temp[from], (to - from + 1) * sizeof(int));
}

void mergeSortPar(int arr[], int temp[], int n) {
    for (int curr_size=1; curr_size<=n-1; curr_size = 2*curr_size) { //cannot parallelize this loop because of invalid inc expression :(
        #pragma omp parallel for
        for (int left_start=0; left_start<n-1; left_start += 2*curr_size) { 
            int mid = left_start + curr_size - 1; 
            int right_end = min(left_start + 2*curr_size - 1, n-1); 
            mergePar(arr, temp, left_start, mid, right_end, n);
       } 
    }
}


bool isOrdered(int array[], int arraySize) {
    for (int i = 1; i < arraySize; ++i) {
        if (array[i] < array[i-1]) return false;
    }
    return true;
}
