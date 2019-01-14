//mpic++ nqueens.cpp -o nqueens -std=c++11 -Wall -Wextra -O3 -march=native
//mpirun -np "processes" ./nqueens n

#include <iostream>
#include <mpi.h>

#include "chrono_timer.h"

using namespace std;

bool is_safe_Pos(int curr_queens,int queen_pos,int* positions){
    for (int i = 0; i < curr_queens; ++i) {
        int row_pos = positions[i];
        if (row_pos == queen_pos ||
            row_pos == queen_pos - (curr_queens - i) ||
            row_pos == queen_pos + (curr_queens - i) )
            return false;
    }
    return true;
}

int solve_rec_seq(int max_queens, int row, int* positions){
    if(max_queens == row)
        return 1;
    int count = 0;
    for (int i = 0; i < max_queens; ++i)
        if (is_safe_Pos(row,i,positions)){
            positions[row] = i;
            count += solve_rec_seq(max_queens,row+1,positions);
        }
    return count;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " N" << endl;
        exit(EXIT_FAILURE);
    }
    unsigned int n = atoi(argv[1]);

    MPI_Init(&argc, &argv);

    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    
    int globalCount = 0;
    {
		int localCount = 0;
		int positions[n];
		ChronoTimer t("Calc & Reduce");
		for (unsigned int i = 0; i < n; ++i) {
			if (i%size != rank){ // Each process does at least n//procs iterations, lower ranks soak up whatever is left
				continue;
			}
			//cout << "Process " << rank << " performing iteration " << i << endl; // DEBUG
			positions[0] = i;
			localCount += solve_rec_seq(n, 1, positions); 
		}
		MPI_Reduce(&localCount, &globalCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	}

    if (rank == 0){
        cout << "Solution for N=" << n << ": " << globalCount << endl;
	}
	
    MPI_Finalize();
    return EXIT_SUCCESS;
}
