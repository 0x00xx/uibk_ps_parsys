# Results

### Rigs
All tests are performed on the **University cluster**

### Benchmark Settings

- 2D grid of 512x512 cells
- 5 point heat stencil
- **`double`** datatype
- Iterate until Epsilon (as the sum of all changes across the grid) is less than **10.0**
- North/East/South/West boundaries fixed at **1.0, 0.5, 0.0 and -0.5** respectively
- All initial values should be set to **0.0**
- Use the **-O3 and -march=native** compiler flags (not -Ofast)

With 1, 2, 4 and 8 threads on a single node of the cluster.

Benchmark times are the **median** of 10 runs achieved in various testing in milliseconds.

## 5 Point Heat 2D Stencil

#### gcc 5.1.0

| N | seq | 1 | 2 | 4 | 8 |
|------|-------|--------|--------|--------|--------|
| 512x512 | 673 | 672.5 | 339.5 | 170 | 1352.5 |

#### gcc 8.2.0

| N | seq | 1 | 2 | 4 | 8 |
|------|-------|--------|--------|--------|--------|
| 512x512 | 668.5 | 671.5 | 336 | 170 | 89 |

_Times in ms_

High StdDev when running on 8 threads -> Cache?

_Compiler Command:_  g++ Stencil.cpp StencilMain.cpp Stencil.h -o stencil -O3 -march=native -Wall -fopenmp
