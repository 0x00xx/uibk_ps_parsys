#include "MergeSort.h"


using namespace std;

int main(int argc, char** argv) {

    if (argc < 2) return EXIT_FAILURE;
    int arraySize = atoi(argv[1]);
    int array[atoi(argv[1])];

    srand (time(NULL));
    for (int i = 0; i < arraySize; ++i) {
        array[i] = rand() % arraySize*2;
    }

    if (argc == 3) {
        cout << "Parallel Version" << endl;
        mergeSortPar(array, arraySize);
    } else {
        cout << "Sequential Version" << endl;
        mergeSortSeq(array, arraySize);
    }
    cout << (isOrdered(array, arraySize) ? "Array is sorted" : "Array is not sorted") << endl;

    return EXIT_SUCCESS;
}
