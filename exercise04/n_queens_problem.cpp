
#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

int numberOfResults = 0;

//print solution
void printSolution(vector<vector<int>> board) { 
	static int k = 1; 
	cout<<k++<< "-\n"; 
	for (int i = 0; i < board.size(); i++) { 
		for (int j = 0; j < board.size(); j++) 
			cout<<" "<< board[i][j]<<" "; 
		cout<<"\n"; 
	} 
	cout<<"\n"; 
} 
  
/*
 * looks if a queen can be placed on this position (checks diagonals and 
 * column)
 */
bool isSafe(vector<vector<int>> board, int row, int col) { 
  
	/* Check this row on left side */
	//#pragma omp parallel
	for (int i = 0; i < col; i++) {
		//#pragma omp critical
		if (board[row][i]) {
			return false; 
		}
	}
	
	/* Check upper diagonal on left side */
	//#pragma omp parallel
	for (int i=row, j=col; i>=0 && j>=0; i--, j--) {
		//#pragma omp critical
		if (board[i][j]) {
			return false; 
		}
	}
  
	/* Check lower diagonal on left side */
	//#pragma omp parallel
	for (int i=row, j=col; j>=0 && i<board.size(); i++, j--){ 
		//#pragma omp critical
		if (board[i][j]) {
			return false; 
		}
	}
	return true; 
} 
  
/* A recursive utility function to solve N 
Queen problem */
bool solveNQUtil(vector<vector<int>> board, int col) { 
	
	/* all queens are placed*/
	if (col == board.size()) { 
		//printSolution(board); 
		numberOfResults++; 
		return true;
	} 
  
	/* Consider column and try placing this queen in all rows*/
	bool res = false; 
	for (int i = 0; i < board.size(); i++) { 
		if ( isSafe(board, i, col) ) { 
			board[i][col] = 1; 
			if(solveNQUtil(board, col + 1) || res){
				res = true;
			} 
			board[i][col] = 0; // BACKTRACK 
		} 
	} 
	return res; 
} 
  
/* This function solves the N Queen problem using 
Backtracking. */
int solveNQ(int N) { 
	vector<vector<int>> board(N, vector<int>(N));;
	
	if (solveNQUtil(board, 0) == false) { 
		printf("Solution does not exist"); 
		return 0; 
	} 
  
	return numberOfResults; 
} 
 
