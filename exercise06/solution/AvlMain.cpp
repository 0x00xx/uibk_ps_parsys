
#include <iostream>
#include "AvlTree.h"
#include <random>
#include "chrono_timer.h"

int main(int argc, char** argv) {
    if(argc!=2 && argc!=3) {
        std::cout << "USAGE ./AvlMain N [par]" << std::endl;
        return EXIT_FAILURE;
    }
    int n = atoi(argv[1]);
    if(n==0)
        return EXIT_FAILURE;

    AvlTree<int> tree;
    int arr[n];
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, n/8);

    for (int i = 0; i < n; ++i) {
        arr[i] = dis(gen);
    }

    if (argc == 3 ){
            ChronoTimer t("Parallel version");
            tree.insertPar(arr,n);
    }
    else {
            ChronoTimer t("Sequential version");
            tree.insertSeq(arr,n);
    }
    std::cout << "Valid? " << (tree.isValid()?"yes":"no") << std::endl;
    std::cout << "Size: " << tree.size() << std::endl;
    if(n<=500)
        tree.printAllValues();
}
