## Compilers to use

The target architecture for all compilers should be x86-64.

- **gcc 8.2** with the options `-std=c++17 -O2 -mavx2`
- **clang 7.0** with the options `-std=c++17 -O2 -mavx2`
- **icc 19** with the options `-std=c++17 -O2 -march=core-avx2`
- **MSVC 19** with the options `/std:c++latest /O2 /arch:AVX2`

## Desired Result

From your investigation, create a table with the compilers as columns and the source/value type configurations as rows. In each cell, mark down the vectorization width (e.g. "4" if 4 double values are computed in one instruction) achieved by the associated compiler for the given configuration.

Note that we only care about the vectorization width of **the main multiplication loop**.

Also investigate whether a compiler performs loop unrolling.

## Results:

### 01_mmul_c_static.c

|        	| gcc 8.2 	| clang 7.0 	| icc 19 	| MSVC 19 	|
|--------	|---------	|-----------	|--------	|---------	|
| double 	|     1    	|      4     	|    2   	|         	|
| int    	|     1    	|      4     	|    1   	|         	|
| float  	|     1    	|      4     	|    2   	|         	|


### 02_mmul_c_dynamic.c

|        	| gcc 8.2 	| clang 7.0 	| icc 19 	| MSVC 19 	|
|--------	|---------	|-----------	|--------	|---------	|
| double 	|         	|           	|        	|         	|
| int    	|         	|           	|        	|         	|
| float  	|         	|           	|        	|         	|


### 03_mmul_cpp_dynamic.c

|        	| gcc 8.2 	| clang 7.0 	| icc 19 	| MSVC 19 	|
|--------	|---------	|-----------	|--------	|---------	|
| double 	|         	|           	|        	|         	|
| int    	|         	|           	|        	|         	|
| float  	|         	|           	|        	|         	|
