# Results

### Rigs
All tests are performed on the **University cluster**

### Benchmark Settings

Testing values: 10k, 20k, 40k, 80k

Both **sequentiell** and **parallel**

Benchmark times are the **median** of 10 runs achieved in various testing in seconds.

## AVLTree

#### No Segmentation Fault:

| N | seq | 1 | 2 | 4 | 8 |
|------|-------|--------|--------|--------|--------|
| 10,000 | 20 | 21 | 30 | 36 | 47.5 |
| 20,000 | 84.5 | 87 | 104.5 | 119 | 141.5 |
| 40,000 | 351 | 356 | 397 | 424.5 | 468.5 |
| 80,000 | 1421.5 | 1433 | 1527.5 | 1566.5 | 1687 |

_Times in ms_

_Compiler Command:_ g++ AvlTree.cpp AvlTree.h AvlNode.cpp AvlNode.h AvlMain.cpp -o avltree -std=c++11 -Wall -Wextra -O3 -fopenmp
