#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>


#include "MergeSort.h"


int main(int argc, char** argv) {
	int N = atoi(argv[1]);
	int* A = new int[N];
    int* tmp = new int[N];
	srand(time(NULL));

	for (int i = 0; i < N; i++)
		tmp[i] = A[i] = rand();

    if (argc == 2) {
	    mergeSortSeq(A, tmp, N);
    } else {
        mergeSortPar(A, tmp, N);
    }

	delete[] A;
	return 0;
    //return isOrdered(A, N);
}
