#include "MergeSort.h"


using namespace std;
void merge(int arr[], int l, int m, int r);
void mergePar(int arr[], int l, int m, int r);

void mergeSortSeq(int arr[], int n) {
    int curr_size;
    int left_start; 
    // Merge subarrays in bottom up manner.  First merge subarrays of 
    // size 1 to create sorted subarrays of size 2, then merge subarrays 
    // of size 2 to create sorted subarrays of size 4, and so on. 
    for (curr_size=1; curr_size<=n-1; curr_size = 2*curr_size) { 
        // Pick starting point of different subarrays of current size 
        for (left_start=0; left_start<n-1; left_start += 2*curr_size) { 
            // Find ending point of left subarray. mid+1 is starting  
            // point of right 
            int mid = left_start + curr_size - 1; 
            int right_end = min(left_start + 2*curr_size - 1, n-1); 
  
            // Merge Subarrays arr[left_start...mid] & arr[mid+1...right_end] 
            merge(arr, left_start, mid, right_end); 
        } 
    }
}

//merge two arrays arr[l..m] and arr[m+1..r]
void merge(int arr[], int l, int m, int r) { 
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 =  r - m; 
    int L[n1], R[n2]; 
  
    //Copy data to temp arrays L[] and R[]
    for (i = 0; i < n1; i++) 
        L[i] = arr[l + i]; 
    for (j = 0; j < n2; j++) 
        R[j] = arr[m + 1+ j]; 
  
    //Merge the temp arrays back into arr[l..r]
    i = 0; 
    j = 0; 
    k = l; 
    while (i < n1 && j < n2) { 
        if (L[i] <= R[j]) { 
            arr[k] = L[i]; 
            i++; 
        } else { 
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
  
    // Copy the remaining elements of L[], if there are any
    while (i < n1) { 
        arr[k] = L[i]; 
        i++; 
        k++; 
    } 
  
    // Copy the remaining elements of R[], if there are any
    while (j < n2) { 
        arr[k] = R[j]; 
        j++; 
        k++; 
    } 
}

void mergeSortPar(int arr[], int n) {
    int curr_size;
    int left_start; 

    
    for (curr_size=1; curr_size<=n-1; curr_size = 2*curr_size) { //cannot parallelize this loop because of invalid inc expression :(
        #pragma omp parallel for
        for (left_start=0; left_start<n-1; left_start += 2*curr_size) { 
            int mid = left_start + curr_size - 1; 
            int right_end = min(left_start + 2*curr_size - 1, n-1); 
            mergePar(arr, left_start, mid, right_end); 
       } 
   }
}

void mergePar(int arr[], int l, int m, int r) { 
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 =  r - m; 
    int L[n1], R[n2]; 
  
    #pragma omp parallel for
    for (i = 0; i < n1; i++) 
        L[i] = arr[l + i];
    #pragma omp parallel for 
    for (j = 0; j < n2; j++) 
        R[j] = arr[m + 1+ j]; 
  
    i = 0; 
    j = 0; 
    k = l; 
    while (i < n1 && j < n2) { 
        if (L[i] <= R[j]) { 
            arr[k] = L[i]; 
            i++; 
        } else { 
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
  
    int k2 = k + n1 - i;
    #pragma omp parallel num_threads(2)
    {
        #pragma omp task
        {
            while (i < n1) { 
                arr[k] = L[i]; 
                i++; 
                k++; 
            } 
        }
       
        #pragma omp task
        {
            while (j < n2) { 
                arr[k2] = R[j]; 
                j++; 
                k2++; 
            } 
        }
    }
}


bool isOrdered(int array[], int arraySize) {
    for (int i = 1; i < arraySize; ++i) {
        if (array[i] < array[i-1]) return false;
    }
    return true;
}
