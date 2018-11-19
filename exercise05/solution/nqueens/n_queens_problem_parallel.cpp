#include <iostream>  // std::cout, cin, cerr ...
#include <iomanip>   // modify std::out
#include <omp.h>

int numberOfSolutions = 0;
int size = 0;

void setQueen_parallel(int queens[], int row, int col, int id) {
	//check all previously placed rows for attacks
	for(int i=0; i<row; i++) {
	   // vertical attacks
	   if (queens[i]==col) {
		   return;
	   }

	   // diagonal attacks
	   if (abs(queens[i]-col) == (row-i) ) {
		  return;
	   }
	}

	// column is ok, set the queen
	queens[row]=col;
	if(row==size-1) {
		#pragma omp atomic
		numberOfSolutions++;  //Placed final queen, found a solution
	}else {
		 // try to fill next row
		 for(int i=0; i<size; i++) {
			 setQueen_parallel(queens, row+1, i, id);
		 }
	}
}

//Function to find all solutions for nQueens problem on size x size chessboard.
int solve_parallel(int N) {
	size = N;
	int myid = 0;
	#pragma omp parallel
	#pragma omp master
	{
		for(int i=0; i<size; i++) {
			myid = omp_get_thread_num(); 
			// try all positions in first row
			#pragma omp task
			setQueen_parallel(new int[size], 0, i, myid);
		 }
	 }
     return numberOfSolutions;
}
