#include <vector>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <mpi.h>
#include "chrono_timer.h"
#include <omp.h>

#define VALUE double

using Matrix = std::vector<std::vector<VALUE>>;

// initializes a square identity matrix of size n x n
Matrix id(unsigned n) {
	Matrix res;
	res.resize(n);
	for(unsigned i=0; i<n; i++) {
		res[i].resize(n);
		for(unsigned j=0; j<n; j++) {
			res[i][j] = (i == j) ? 1 : 0;
		}
	}
	return res;
}

Matrix zero(unsigned n) {
	Matrix res;
	res.resize(n);
	for(unsigned i=0; i<n; i++) {
		res[i].resize(n);
		for(unsigned j=0; j<n; j++) {
			res[i][j] = 0;
		}
	}
	return res;
}

int x = 0;
int bound = 0;
std::vector<VALUE> toSend = std::vector<VALUE>();

// computes the product of two matrices
Matrix operator*(const Matrix& a, const Matrix& b) {
	unsigned n = a.size();
	Matrix c = zero(n);
    int y = 0;
    #pragma omp parallel for
	for(unsigned i=x; i<bound; ++i) {
		for(unsigned j=0; j<n; ++j) {
			for(unsigned k=0; k<n; ++k) {
				c[i][j] += a[i][k] * b[k][j];
			}
			#pragma omp ordered
            toSend.push_back(c[i][j]);
		}
	}
	return c;
}

void printMatrix(const Matrix& a) {
    std::cout << "----------" << std::endl;
    unsigned n = a.size();
    for(unsigned i=0; i<n; i++) {
		for(unsigned j=0; j<n; j++) {
			std::cout << a[i][j] << " ";
		}
        std::cout << std::endl;
	}
}


int main(int argc, char** argv) {
	
	if(argc!=2) return EXIT_FAILURE;
	unsigned n = atoi(argv[1]);
	if(n==0) return EXIT_FAILURE;

	// create two matrices
	auto a = id(n);
	a[0][0] = 42;
	auto b = id(n);

    MPI_Init(&argc, &argv);

    // Get the number of processes
    int numProc;
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    Matrix c;
    {
		MPI_Barrier(MPI_COMM_WORLD);
	    ChronoTimer t("Time");

	    if (rank == 0) {
	        x = 0;
	        bound = (n / numProc);
	    } else if (rank == numProc - 1 && (n % numProc) != 0) {
	        x = (n / numProc) * rank;
	        bound = x + (n / numProc)+1;
	    } else {
	        x = (n / numProc) * rank;
	        bound = x + (n / numProc);
	    }

		// compute the product
		c = a * b;

	    if (rank != 0) {
	        MPI_Request req;
	        MPI_Isend(&toSend[0], toSend.size(), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &req);
	    } else {
	        for (int l = 1; l < numProc; l++) {
	            MPI_Status status;
	            MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	            int senderRank = status.MPI_SOURCE;

	            int row = (n / numProc);
	            if (senderRank == numProc - 1 && (n % numProc) != 0) {
	                row++;
	            }
	            std::vector<VALUE> buff = std::vector<VALUE>();
	            buff.resize(n * row);
	            MPI_Recv(&buff[0], n * row, MPI_DOUBLE, senderRank, 0, MPI_COMM_WORLD, &status);
	            int y = (n / numProc) * senderRank;
	            int z = 0;
	            for (int i = 0; i < buff.size(); ++i) {
	               c[y][z] = buff[i];
	               z++;
	               if (z == n) {
	                z = 0;
	                y++;
	               } 
	            }
	        }
	    }

	    MPI_Barrier(MPI_COMM_WORLD);
	    MPI_Finalize();
	}
/*
	// check that the result is correct
    if (rank == 0) {
        if (c == a) {
            std::cout << "Worked\n";
        } else {
            std::cout << "Not\n";
        }
    }
*/
	return EXIT_SUCCESS;
}


