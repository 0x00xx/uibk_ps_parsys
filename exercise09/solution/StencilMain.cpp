
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

    MPI_Init(&argc, &argv);

    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<double> *in;
    std::vector<double> *out;
    std::vector<double> *result;
    MPI_Status status;

    switch (dim) {
    case 1:
        if (size == 1) { //sequential
            in = new std::vector<double>(n);
            setBoundaries(2, n, *bounds, *in);
            out = new std::vector<double>(n);
            std::copy(in->begin(), in->end(), out->begin());
        } else if (rank == 0) { //first item
            int tmpSize = (n/size) + ghosts;
            in = new std::vector<double>(tmpSize);
            result = new std::vector<double>(n);
            setBoundaries(1, n, *bounds, *result);
            for (int i = 1; i < size-1; ++i) {
                MPI_Send(&result->at((n/size)*i-ghosts), (n/size)+2*ghosts,
                         MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
            }
            MPI_Send(&result->at((n/size)*(size-1)-ghosts), (n/size)+ghosts,
                     MPI_DOUBLE, size-1, 1, MPI_COMM_WORLD);
            out = new std::vector<double>(tmpSize);
            std::copy(result->begin(), result->begin()+ghosts+(n/size), in->begin());
            std::copy(in->begin(), in->end(), out->begin());
        } else if (rank == size - 1) { //last item
            int tmpSize = (n/size) + ghosts;
            in = new std::vector<double>(tmpSize);
            MPI_Recv(&in->at(0), tmpSize, MPI_DOUBLE,
                     0, 1, MPI_COMM_WORLD, &status);
            out = new std::vector<double>(tmpSize);
            std::copy(in->begin(), in->end(), out->begin());
        } else { //middle items
            int tmpSize = (n/size) + 2*ghosts;
            in = new std::vector<double>(tmpSize);
            MPI_Recv(&in->at(0), tmpSize, MPI_DOUBLE,
                     0, 1, MPI_COMM_WORLD, &status);
            out = new std::vector<double>(tmpSize);
            std::copy(in->begin(), in->end(), out->begin());
        }
        if (size < 2) {
            ChronoTimer t("1D Serial version");
            jacobi1DSeq(*bounds, epsilon, in, out);
        } else {
            ChronoTimer t("1D Parallel version");
            in = jacobi1DPar(*bounds, epsilon, n, in, out, rank, size, ghosts);
            std::cout << rank << " ends here" << std::endl;

            int offset = rank==0?0:ghosts;
            double *results;
            if(rank==0)
                results = (double*) malloc(n*sizeof(double));
            MPI_Gather(&in->at(offset), (n/size), MPI_DOUBLE, results, (n/size), MPI_DOUBLE, 0, MPI_COMM_WORLD);// Get all results

            if (rank == 0) {
                for (int i = 0; i < n; ++i) {
                    result->at(i) = results[i];
                }
                //std::cout << "Print" << std::endl;
                //print_Array(dim, result, n);
            }
        }
        break;
    case 2:
        if (size == 1) { //sequential
            in = new std::vector<double>(n*n);
            setBoundaries(2, n, *bounds, *in);
            out = new std::vector<double>(n*n);
            std::copy(in->begin(), in->end(), out->begin());
        } else if (rank == 0) { //first item
            int tmpSize = n*(n/size) + ghosts*n;
            in = new std::vector<double>(tmpSize);
            result = new std::vector<double>(n*n);
            setBoundaries(2, n, *bounds, *result);
            for (int i = 1; i < size-1; ++i) {
                MPI_Send(&result->at(n*(n/size)*i-ghosts*n), n*(n/size)+2*ghosts*n,
                         MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
            }
            MPI_Send(&result->at(n*(n/size)*(size-1)-ghosts*n), n*(n/size)+ghosts*n,
                     MPI_DOUBLE, size-1, 1, MPI_COMM_WORLD);
            out = new std::vector<double>(tmpSize);
            std::copy(result->begin(), result->begin()+n*ghosts+n*(n/size), in->begin());
            std::copy(in->begin(), in->end(), out->begin());
        } else if (rank == size - 1) { //last item
            int tmpSize = n*(n/size) + ghosts*n;
            in = new std::vector<double>(tmpSize);
            MPI_Recv(&in->at(0), tmpSize, MPI_DOUBLE,
                     0, 1, MPI_COMM_WORLD, &status);
            out = new std::vector<double>(tmpSize);
            std::copy(in->begin(), in->end(), out->begin());
        } else { //middle items
            int tmpSize = n*(n/size) + 2*ghosts*n;
            in = new std::vector<double>(tmpSize);
            MPI_Recv(&in->at(0), tmpSize, MPI_DOUBLE,
                     0, 1, MPI_COMM_WORLD, &status);
            out = new std::vector<double>(tmpSize);
            std::copy(in->begin(), in->end(), out->begin());
        }
        if (size < 2) {
            ChronoTimer t("2D Serial version");
            jacobi2DSeq(*bounds, epsilon, n, in, out);
        } else {
            ChronoTimer t("2D Parallel version");
            in = jacobi2DPar(*bounds, epsilon, n, in, out, rank, size, ghosts);
            std::cout << rank << " ends here" << std::endl;

            int offset = rank==0?0:ghosts*n;
            double *results;
            if(rank==0)
                results = (double*) malloc(n*n*sizeof(double));
            MPI_Gather(&in->at(offset), n*(n/size), MPI_DOUBLE, results, n*(n/size), MPI_DOUBLE, 0, MPI_COMM_WORLD);// Get all results

            if (rank == 0) {
                for (int i = 0; i < n*n; ++i) {
                    result->at(i) = results[i];
                }
                //std::cout << "Print" << std::endl;
                //print_Array(dim, result, n);
            }
        }
        break;
    case 3:
        if (size == 1) { //sequential
            in = new std::vector<double>(n*n*n);
            setBoundaries(3, n, *bounds, *in);
            out = new std::vector<double>(n*n*n);
            std::copy(in->begin(), in->end(), out->begin());
        } else if (rank == 0) { //first item
            int tmpSize = n*n*(n/size) + ghosts*n*n;
            in = new std::vector<double>(tmpSize);
            result = new std::vector<double>(n*n*n);
            setBoundaries(3, n, *bounds, *result);
            for (int i = 1; i < size-1; ++i) {
                MPI_Send(&result->at(n*n*(n/size)*i-ghosts*n*n), n*n*(n/size)+2*ghosts*n*n,
                         MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
            }
            MPI_Send(&result->at(n*n*(n/size)*(size-1)-ghosts*n*n), n*n*(n/size)+ghosts*n*n,
                     MPI_DOUBLE, size-1, 1, MPI_COMM_WORLD);
            out = new std::vector<double>(tmpSize);
            std::copy(result->begin(), result->begin()+n*n*ghosts+n*n*(n/size), in->begin());
            std::copy(in->begin(), in->end(), out->begin());
        } else if (rank == size - 1) { //last item
            int tmpSize = n*n*(n/size) + ghosts*n*n;
            in = new std::vector<double>(tmpSize);
            MPI_Recv(&in->at(0), tmpSize, MPI_DOUBLE,
                     0, 1, MPI_COMM_WORLD, &status);
            out = new std::vector<double>(tmpSize);
            std::copy(in->begin(), in->end(), out->begin());
        } else { //middle items
            int tmpSize = n*n*(n/size) + 2*ghosts*n*n;
            in = new std::vector<double>(tmpSize);
            MPI_Recv(&in->at(0), tmpSize, MPI_DOUBLE,
                     0, 1, MPI_COMM_WORLD, &status);
            out = new std::vector<double>(tmpSize);
            std::copy(in->begin(), in->end(), out->begin());
        }
        if (size < 2) {
            ChronoTimer t("3D Serial version");
            jacobi3DSeq(*bounds, epsilon, n, in, out);
        } else {
            ChronoTimer t("3D Parallel version");
            in = jacobi3DPar(*bounds, epsilon, n, in, out, rank, size, ghosts);
            std::cout << rank << " ends here" << std::endl;

            int offset = rank==0?0:ghosts*n*n;
            double *results;
            if(rank==0)
                results = (double*) malloc(n*n*n*sizeof(double));
            MPI_Gather(&in->at(offset), n*n*(n/size), MPI_DOUBLE, results, n*n*(n/size), MPI_DOUBLE, 0, MPI_COMM_WORLD);// Get all results

            if (rank == 0) {
                for (int i = 0; i < n*n*n; ++i) {
                    result->at(i) = results[i];
                }
                //std::cout << "Print" << std::endl;
                //print_Array(dim, result, n);
            }
        }
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
