#include <iostream>
#include <vector>
#include <random>
#include "Stencil.h"
#include "chrono_timer.h"

constexpr int default_bounds[]{1, 1, 1, 1, 1, 1};

void print_Array(int dim, std::vector<double>* arr, int n );

int main(int argc, char **argv) {
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " EPSILON DIM N THREADS [BOUND...]" << std::endl;
        exit(EXIT_FAILURE);
    }

    double epsilon = atof(argv[1]);
    unsigned int dim = atoi(argv[2]);
    unsigned long n = atol(argv[3])+2;
    unsigned int threads = atoi(argv[4]);

    // Allocate memory for the bounds depending on the problem dimensions
    std::vector<int> *bounds;
    switch (dim) {
    case 1:
        bounds = new std::vector<int>(2);
        break;
    case 2:
        bounds = new std::vector<int>(4);
        break;
    case 3:
        bounds = new std::vector<int>(6);
        break;
    default:
        std::cerr << "Invalid dimension" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Override default bounds from command-line if specified
    for (unsigned long i = 0; i < bounds->size(); ++i) {
        auto arg = 5 + i;
        if (argc > arg)
            bounds->at(i) = atol(argv[arg]);
        else
            bounds->at(i) = default_bounds[i];
    }

    // allocate the in and output vector and initialize it
    std::vector<double> *in;
    std::vector<double> *out;

    switch (dim) {
    case 1:
        in = new std::vector<double>(n);
        setBoundaries(1, n, *bounds, *in);
        out = new std::vector<double>(n);
        std::copy(in->begin(), in->end(), out->begin());

        if(threads<2) {
            ChronoTimer t("1D Serial version");
            jacobi1DSeq(*bounds,epsilon,in,out);
        } else {
            ChronoTimer t("1D Parallel version");
            jacobi1DPar(*bounds,epsilon,in,out);
        }
        break;

    case 2:
        in = new std::vector<double>(n*n);
        setBoundaries(2, n, *bounds, *in);
        out = new std::vector<double>(n*n);
        std::copy(in->begin(), in->end(), out->begin());

        if(threads<2) {
            ChronoTimer t("2D Serial version");
            jacobi2DSeq(*bounds,epsilon,n,in,out);
        } else {
            ChronoTimer t("2D Parallel version");
            jacobi2DPar(*bounds,epsilon,n,in,out);
        }
        break;

    case 3:
        in = new std::vector<double>(n*n*n);
        setBoundaries(3, n, *bounds, *in);
        out = new std::vector<double>(n*n*n);
        std::copy(in->begin(), in->end(), out->begin());

        if(threads<2) {
            ChronoTimer t("3D Serial version");
            jacobi3DSeq(*bounds,epsilon,n,in,out);
        } else {
            ChronoTimer t("3D Parallel version");
            jacobi3DPar(*bounds,epsilon,n,in,out);
        }
        break;
    }
}

void print_Array(int dim, std::vector<double>* arr, unsigned long n){
    switch (dim) {
    case 1:
        for (unsigned long i = 0; i < n; ++i) {
            std::cout << arr->at(i) << " ";
        }
        std::cout << std::endl;
        break;
    case 2:
        for (unsigned long j = 0; j < n; ++j) {
            for (unsigned long i = 0; i < n; ++i) {
                std::cout << arr->at(n*j+i) << " ";
            }
            std::cout << std::endl;
        }

        break;
    case 3:
        for (unsigned long k = 0; k < n; ++k) {
            std::cout << "Step:" << k << std::endl;
            for (unsigned long j = 0; j < n; ++j) {
                for (unsigned long i = 0; i < n; ++i) {
                    std::cout << arr->at(n*n*k+n*j + i) << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
        break;
    }

}
