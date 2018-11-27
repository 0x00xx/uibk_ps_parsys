# Results

### Rigs
All tests are performed on the **University cluster**

### Benchmark Settings

Testing values: 10k, 20k, 40k, 80k

Both **sequentiell** and **parallel**

Benchmark times are the **median** of 10 runs achieved in various testing in seconds.

## AVLTree

| N | seq | 1 | 2 | 4 | 8 |
|------|-------|--------|--------|--------|--------|
| 10,000 | | | | | |
| 20,000 | | | | | |
| 40,000 | | | | | |
| 80,000 | | | | | |

_Compiler Command:_ g++ AvlTree.cpp AvlTree.h AvlNode.cpp AvlNode.h AvlMain.cpp -o avltree -std=c++11 -Wall -Wextra -O3 -fopenmp
