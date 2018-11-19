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

Benchmark times is the **minimum** of 10 runs achieved in various testing.

## N-Queens Problem

### Original

| size\threads | seq | 1 | 2 | 4 | 8 |
|------|-------|--------|--------|--------|--------|
| **8**   | | | | | |
| **10**   | | | | | |
| **12**   | | | | | |

_Seq. Compiler Command:_

_Par. Compiler Command:_

### Optimized

| size\threads | seq | 1 | 2 | 4 | 8 |
|------|-------|--------|--------|--------|--------|
| **8**   | | | | | |
| **10**   | | | | | |
| **12**   | | | | | |

_Seq. Compiler Command:_

_Par. Compiler Command:_

## Montecarlo

### Original

| size\threads | seq | 1 | 2 | 4 | 8 |
|------|-------|--------|--------|--------|--------|
| **1,000,000**   | | | | | |
| 10,000,000   | | | | | |
| 100,000,000   | | | | | |

_Seq. Compiler Command:_

_Par. Compiler Command:_

### Optimized

| size\threads | seq | 1 | 2 | 4 | 8 |
|------|-------|--------|--------|--------|--------|
| **1,000,000**   | | | | | |
| 10,000,000   | | | | | |
| 100,000,000   | | | | | |

_Seq. Compiler Command:_

_Par. Compiler Command:_

## Mergesort

### Original

| size\threads | seq | 1 | 2 | 4 | 8 |
|------|-------|--------|--------|--------|--------|
| **100,000,000**   | | | | | |
| 200,000,000   | | | | | |
| 400,000,000   | | | | | |

_Seq. Compiler Command:_

_Par. Compiler Command:_

### Optimized

| size\threads | seq | 1 | 2 | 4 | 8 |
|------|-------|--------|--------|--------|--------|
| **100,000,000**   |23.6172471046|23.3746769428|13.6242659092|9.16054105759|8.83927297592|
| 50,000,000   |11.3487398624|11.2156360149|6.56290817261|4.41468501091|4.26440906525|
| 10,000,000   |2.04000997543|2.01568889618|1.21888995171|0.808686971664|0.773750066757|


_Seq. Compiler Command:_

_Par. Compiler Command:_
