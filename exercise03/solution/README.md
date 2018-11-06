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

|        	| gcc 8.2 	| clang 7.0 	| icc 19 	| MSVC 19 2017 RTW 	|
|--------	|---------	|-----------	|--------	|---------	|
| double 	|     1    	|      2x4     	|    2   	|     2x2    	|
| int    	|     1    	|      4x4     	|    4   	|     4x2    	|
| float  	|     1    	|      4x4     	|    4   	|     4x2    	|


### 02_mmul_c_dynamic.c

|        	| gcc 8.2 	| clang 7.0 	| icc 19 	| MSVC 19 	|
|--------	|---------	|-----------	|--------	|---------	|
| double 	|     1    	|     2x4      	|    2    	|     1x4    	|
| int    	|     1    	|     4x4      	|    4    	|      1   	|
| float  	|     1    	|     4x4      	|    4    	|     1x4    	|


### 03_mmul_cpp_dynamic.c

|        	| gcc 8.2 	| clang 7.0 	| icc 19 	| MSVC 19 	|
|--------	|---------	|-----------	|--------	|---------	|
| double 	|     1    	|      1x2     	|   1x4     	|    1x4     	|
| int    	|     1    	|      1x2     	|   1x2     	|    1     	|
| float  	|     1    	|      1x2     	|   1x8     	|    1x4     	|


xN denotes loop unrolling.

- ICC 01 double -> operation on packed doubles (2 cause 128 bit) 
- ICC 01 int -> vpmulld: multiplies 4 ints
- ICC 01 float -> see double variant: replaced all double operations with float operations
- ICC 02 -> analog to ICC 01
- ICC 03 double -> like ICC 01 but doesn't use packed-double operations anymore (just low packed) but it uses unrolling
- ICC 03 int -> use imul instead of vpmull
- Clang 01 double -> 4times vmulpd/vaddpd/vmovupd ... pd double packages (2 doubles)
- Clang 01 int -> 4times vpmull/vpaddd/vmovdqu
- Clang 01 float -> see double variant:  replaced all double operations with float operations
- Clang 02 -> analog to Clang 01
- Clang 03 double -> see ICC 03 double (nearly the same code)
- Clang 03 int -> doesn't use vector operations anymore
- MSVC 01 double-> computes 2 double values at the same time and unrolls a loop twice
- MSVC 01 int/float -> takes 4 numbers at once and unrolls as well twice
- MSVC 02 double/float -> only calculates 1 number but unrolls the loop 4 times
- MSVC 02 int -> doesn't unroll at all
- MSVC 03 -> same as MSVC 02

