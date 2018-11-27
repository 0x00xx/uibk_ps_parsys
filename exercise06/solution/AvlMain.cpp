
#include <iostream>
#include "AvlTree.h"
#include <random>
#include <vector>
#include <algorithm>
#include <numeric>
#include "chrono_timer.h"

bool mysort(double i,double j) { return (i<j); }

double median(std::vector<int> scores)
{
  size_t size = scores.size();
  if (size == 0)
  {
    return 0;  // Undefined, really.
  }
  else
  {
    sort(scores.begin(), scores.end());
    if (size % 2 == 0)
    {
      return ((double)scores[size / 2 - 1] + (double)scores[size / 2]) / 2;
    }
    else 
    {
      return scores[size / 2];
    }
  }
}

int main(int argc, char** argv) {
    if(argc!=2 && argc!=3) {
        std::cout << "USAGE ./AvlMain N [par]" << std::endl;
        return EXIT_FAILURE;
    }
    int n = atoi(argv[1]);
    if(n==0)
        return EXIT_FAILURE;
    int result_size = 10;
    int ms[result_size];

    for (int i = 0; i < result_size; ++i) {
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
                std::cout<< "Timer - " << "Parallel version" << ": " << t.getElapsedTime().count() << " ms " << std::endl;
                ms[i] = t.getElapsedTime().count();
        }
        else {
                ChronoTimer t("Sequential version");
                tree.insertSeq(arr,n);
                std::cout<< "Timer - " << "Parallel version" << ": " << t.getElapsedTime().count() << " ms " << std::endl;
                ms[i] = t.getElapsedTime().count();
        }
        std::cout << "Valid? " << (tree.isValid()?"yes":"no") << std::endl;
        std::cout << "Size: " << tree.size() << std::endl;
    }
    std::vector<int> result_vector (ms, ms+result_size);
    std::sort (result_vector.begin(), result_vector.end(), mysort);
    std::cout << "Timestamps for this run:";
    for (std::vector<int>::iterator it=result_vector.begin(); it!=result_vector.end(); ++it)
        std::cout << ' ' << *it;
        std::cout << '\n';
    std::cout << "Median: " << median(result_vector) << " ms" << std::endl;
    std::cout << "Average: " << (accumulate( result_vector.begin(), result_vector.end(), 0.0)/result_vector.size()) << " ms" << std::endl;
    std::cout << "Minimum: " << result_vector.front() << " ms" << std::endl;
    std::cout << "Maximum: " << result_vector.back() << " ms" << std::endl;
}
