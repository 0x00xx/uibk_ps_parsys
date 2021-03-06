# Results

### Rigs
All tests are performed on the **University cluster**

### Benchmark Settings

Testing values:

| N-Queens | Montecarlo | Mergesort |
|----------|------------|-----------|
| **8**       | **1,000,000**   | **100,000,000 int32_t**  |
| **10**       | 10,000,000    | 50,000,000 int32_t    |
| **12**       | 100,000,000    | 10,000,000 int32_t      |

**BOLD** is given by the exercise

Both **sequentiell** and **parallel**

Benchmark times is the **minimum** of 10 runs achieved in various testing in seconds.

## N-Queens Problem

### Original

| size\threads | seq | 1 | 2 | 4 | 8 |
|------|-------|--------|--------|--------|--------|
| **8**   | 0.00657 | 0.00693 | 0.00688 | 0.00708 | 0.00722|
| **10**   | 0.01159 | 0.01192 | 0.00948 | 0.00869 | 0.00841 |
| **12**   | 0.17557 | 0.17730 | 0.09252 | 0.05337 | 0.03888 |

### Optimized

| size\threads | seq | 1 | 2 | 4 | 8 |
|------|-------|--------|--------|--------|--------|
| **8**   | 0.00641| 0.00640 | 0.00653 | 0.00668 | 0.00638 |
| **10**   | 0.01114 | 0.01160 | 0.00927 | 0.00833 | 0.00799 |
| **12**   | 0.17544 | 0.18216 | 0.09636 | 0.05359 | 0.03849 |

_Seq. Compiler Command:_ g++ main_seq.cpp -Wall -O3 -std=c++11 -o nqueens

_Par. Compiler Command:_ g++ main_parallel.cpp -fopenmp -Wall -O3 -std=c++11 -o nqueens_par

## Montecarlo

### Original

| size\threads | seq | 1 | 2 | 4 | 8 |
|------|-------|--------|--------|--------|--------|
| **1,000,000**   | N/A | N/A| N/A | N/A | N/A |
| 10,000,000   | N/A | N/A| N/A | N/A | N/A |
| 100,000,000   | N/A | N/A| N/A | N/A | N/A |

_GMP is not installed on the cluster_

### Optimized

| size\threads | seq | 1 | 2 | 4 | 8 |
|------|-------|--------|--------|--------|--------|
| **1,000,000**   | 0.02821 | 0.02251 | 0.01458 | 0.01073 | 0.00862 |
| 10,000,000   | 0.22523 | 0.16723 | 0.08724| 0.04707 | 0.02709 |
| 100,000,000   | 2.19556 | 1.61435 | 0.81155 | 0.40988 | 0.20855 |

_Seq. Compiler Command:_ g++ Main.cpp Montecarlo.h Montecarlo.cpp -fopenmp -Wall -O3 -std=c++11 -o montecarlo

_Par. Compiler Command:_ g++ Main.cpp Montecarlo.h Montecarlo.cpp -fopenmp -Wall -O3 -std=c++11 -o montecarlo

## Mergesort

### Original

| size\threads | seq | 1 | 2 | 4 | 8 |
|------|-------|--------|--------|--------|--------|
| **100,000,000**   | N/A | N/A| N/A | N/A | N/A |
| 50,000,000   | N/A | N/A| N/A | N/A | N/A |
| 10,000,000   | N/A | N/A| N/A | N/A | N/A |

_Memory error_

### Optimized with array check

| size\threads | seq | 1 | 2 | 4 | 8 |
|------|-------|--------|--------|--------|--------|
| **100,000,000**   |23.6172471046|23.3746769428|13.6242659092|9.16054105759|8.83927297592|
| 50,000,000   |11.3487398624|11.2156360149|6.56290817261|4.41468501091|4.26440906525|
| 10,000,000   |2.04000997543|2.01568889618|1.21888995171|0.808686971664|0.773750066757|

### Optimized without array check
| size\threads | seq | 1 | 2 | 4 | 8 |
|------|-------|--------|--------|--------|--------|
| **100,000,000**   |23.0469901562|22.7826499939|13.1207032204|8.63236498833|8.3134200573|
| 50,000,000   |11.0904340744|10.9588308334|6.32691812515|4.1782810688|4.01788687706|
| 10,000,000   |2.02651500702|2.00008797646|1.20089912415|0.794095039368|0.758894920349|


_Seq. Compiler Command:_ g++ Main.cpp MergeSort.h MergeSort.cpp -fopenmp -Wall -O3 -std=c++11 -o merge

_Par. Compiler Command:_ g++ Main.cpp MergeSort.h MergeSort.cpp -fopenmp -Wall -O3 -std=c++11 -o merge
