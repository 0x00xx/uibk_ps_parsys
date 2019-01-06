
#include <iostream>
#include <vector>
#include <random>
#include "Stencil.h"
#include "chrono_timer.h"
#include <unistd.h>

constexpr double default_bounds[]{2.0, 2.0, 2.0, 2.0, 2.0, 2.0};

void print_Array(int dim, std::vector<double> *arr, int n);

int main(int argc, char **argv)
{
    // Check for required args - use threads as args so we can later set the procs for OpenMPI using the same main
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " EPSILON DIM N GHOSTS [BOUND...]" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Parse required args
    double epsilon = atof(argv[1]);
    unsigned int dim = atoi(argv[2]);
    unsigned long n = atol(argv[3]) + 2;
    unsigned int ghosts = atoi(argv[4]);
	if(n%64 == 0){
        std::cerr << "N cannot be divided by 64" << std::endl;
		exit(EXIT_FAILURE);
	}
	if(n%2 != 0){
		n++;
	}
    if (n < 1 || ghosts > n || ghosts < 1 || dim < 1 || dim > 3) {
        std::cerr << "N > 0 && N>Ghosts>0 && 0<dim<4" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Allocate memory for the bounds depending on the problem dimensions
    std::vector<double> *bounds;
    switch (dim) {
    case 1:
        bounds = new std::vector<double>(2);
        break;
    case 2:
        bounds = new std::vector<double>(4);
        break;
    case 3:
        bounds = new std::vector<double>(6);
        break;
    default:
        std::cerr << "Invalid dimension!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Override default bounds from command-line if specified
    for (unsigned long i = 0; i < bounds->size(); ++i) {
        auto arg = 5 + i;
        if (argc > arg) {
            bounds->at(i) = atof(argv[arg]);
        }
        else
            bounds->at(i) = default_bounds[i];
    }

    // allocate the in and output vector and initialize it
    // run program

	std::vector<double> *in;
    std::vector<double> *out;
	in = new std::vector<double>(n*n);
    setBoundaries(2, n, *bounds, *in);
    out = new std::vector<double>(n*n);
    std::copy(in->begin(), in->end(), out->begin());

    MPI_Init(&argc, &argv);

    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    
    MPI_Status status;


    switch (dim) {
    case 1:
        
    case 2:
        
        
        
        if (size < 2) {//sequentiell
            ChronoTimer t("2D Serial version");
            jacobi2DSeq(*bounds, epsilon, n, in, out);
        } else {
			
            ChronoTimer t("2D Parallel version");
            jacobi2DPar(*bounds, epsilon, n, in, out, rank, size);
            std::cout << rank << " ends here" << std::endl;

           
            /*double *results;
            if(rank==0){
                results = (double*) malloc(n*n*sizeof(double));
                for (int i = 0; i < n*n; ++i) {
                    out->at(i) = results[i];
                }
                //std::cout << "Print" << std::endl;
                //print_Array(dim, result, n);
            }*/
        }
        break;
    case 3:
        break;
    }
    MPI_Finalize();
    return EXIT_SUCCESS;
}

void print_Array(int dim, std::vector<double> *arr, int n)
{
    switch (dim) {
    case 1:
        for (int i = 0; i < n; ++i) {
            std::cout << arr->at(i) << " ";
        }
        std::cout << std::endl;
        break;
    case 2:
        for (int j = 0; j < n; ++j) {
            for (int i = 0; i < n; ++i) {
                std::cout << arr->at(n*j + i) << " ";
            }
            std::cout << std::endl;
        }

        break;
    case 3:
        for (int k = 0; k < n; ++k) {
            std::cout << "Step:" << k << std::endl;
            for (int j = 0; j < n; ++j) {
                for (int i = 0; i < n; ++i) {
                    std::cout << arr->at(n*n*k + n*j + i) << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
        break;
    }

}
