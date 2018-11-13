#include "n_queens_problem_seq.cpp"
#include <iostream>

int main(int argc, char** argv){
	if (argc < 2) return EXIT_FAILURE;
    int arraySize = atoi(argv[1]);
    
	std::cout << solve(arraySize) << "\n";
	return 0;
}
