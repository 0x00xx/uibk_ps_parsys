#include "MergeSort.h"


using namespace std;

int main(int argc, char** argv) {

    if (argc < 2) return EXIT_FAILURE;
    int32_t arraySize = atoi(argv[1]);
    int32_t array[atoi(argv[1])];

    srand (time(NULL));
    for (int i = 0; i < arraySize; ++i) {
        array[i] = rand() % arraySize;
    }

    if (argc == 3) {
        mergeSortPar(array, arraySize);
	//mergeSortRecPar(array, 0, arraySize-1);
    } else {
        mergeSortSeq(array, arraySize);
	//mergeSortRecSeq(array, 0, arraySize-1);
    }
    return isOrdered(array, arraySize);
}
