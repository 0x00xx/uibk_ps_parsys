#include "n_queens_problem_parallel.cpp"
#include <iostream>

int main(int argc, char** argv){
	if (argc < 2) return EXIT_FAILURE;
    	int arraySize = atoi(argv[1]);
	solve_parallel(arraySize);
	return 0;
}
