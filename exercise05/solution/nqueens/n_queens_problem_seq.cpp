#include <iostream>  // std::cout, cin, cerr ...
#include <iomanip>   // modify std::out
#include <omp.h>

void setQueen(int queens[], int row, int col, int size, int &numberOfSolutions) {
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
		numberOfSolutions++;  //Placed final queen, found a solution
	}
	else {
		 // try to fill next row
		 for(int i=0; i<size; i++) {
			 setQueen(queens, row+1, i, size, numberOfSolutions);
		 }
	}
}

//Function to find all solutions for nQueens problem on size x size chessboard.
int solve(int size) {
	int numberOfSolutions = 0;
    for(int i=0; i<size; i++) {
         // try all positions in first row
         setQueen(new int[size], 0, i, size, numberOfSolutions);
         delete[](queens);
     }
     return numberOfSolutions;
}
