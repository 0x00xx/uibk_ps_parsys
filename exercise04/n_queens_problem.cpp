
#include <iostream>

#define N 4

using namespace std;
  
  int numberOfResults = 0;
  
//print solution
void printSolution(int board[N][N]) { 
    static int k = 1; 
    cout<<k++<< "-\n"; 
    for (int i = 0; i < N; i++) { 
        for (int j = 0; j < N; j++) 
            cout<<" "<< board[i][j]<<" "; 
        cout<<"\n"; 
    } 
    cout<<"\n"; 
} 
  
/*
 * looks if a queen can be placed on this position (checks diagonals and 
 * column)
 */
bool isSafe(int board[N][N], int row, int col) { 
  
    /* Check this row on left side */
    for (int i = 0; i < col; i++) {
        if (board[row][i]) {
            return false; 
		}
	}
	
    /* Check upper diagonal on left side */
    for (int i=row, j=col; i>=0 && j>=0; i--, j--) {
        if (board[i][j]) {
            return false; 
		}
	}
  
    /* Check lower diagonal on left side */
    for (int i=row, j=col; j>=0 && i<N; i++, j--){ 
        if (board[i][j]) {
            return false; 
		}
	}
    return true; 
} 
  
/* A recursive utility function to solve N 
Queen problem */
bool solveNQUtil(int board[N][N], int col) { 
    
    /* all queens are placed*/
    if (col == N) { 
        //printSolution(board); 
        numberOfResults++; 
        return true;
    } 
  
    /* Consider column and try placing this queen in all rows*/
    bool res = false; 
    for (int i = 0; i < N; i++) { 
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
void solveNQ() { 
    int board[N][N];
    //set Matrix to 0
    for(int i = 0; i<N;i++){
		for(int j = 0; j<N; j++){
			board[i][j] = 0;
		}
	}
  
    if (solveNQUtil(board, 0) == false) { 
        printf("Solution does not exist"); 
        return ; 
    } 
  
    return ; 
} 
  
// driver program to test above function 
int main() { 
    solveNQ(); 
    cout<<"N = "<<N<<"\nNumber of Results: "<<numberOfResults<<"\n";
    return 0; 
} 
