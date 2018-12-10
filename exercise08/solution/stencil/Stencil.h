
#ifndef PARALLEL_STENCIL_H
#define PARALLEL_STENCIL_H

#include <vector>
#include "Math.h"
#include <mpi.h>



//Perform 1-dimensional sequential jacobi iteration algorithm
void jacobi1DSeq(const std::vector<double> &bounds, double epsilon, std::vector<double> *in, std::vector<double> *out);


//Perform 2-dimensional sequential jacobi iteration algorithm
void jacobi2DSeq(const std::vector<double> &bounds, const double epsilon, const unsigned long n, std::vector<double> *in,
                 std::vector<double> *out);


//Perform 3-dimensional sequential jacobi iteration algorithm
void jacobi3DSeq(const std::vector<double> &bounds, const double epsilon, const unsigned long n, std::vector<double> *in,
                 std::vector<double> *out);


//Perform 1-dimensional sequential jacobi iteration algorithm
std::vector<double> * jacobi1DPar(const std::vector<double> &bounds, const double epsilon, const unsigned long n, std::vector<double> *in,
                                  std::vector<double> *out, int rank, int size, int ghosts);

//Perform 2-dimensional sequential jacobi iteration algorithm
std::vector<double> * jacobi2DPar(const std::vector<double> &bounds, const double epsilon, const unsigned long n, std::vector<double> *in,
                 std::vector<double> *out, int rank, int size, int ghosts);


//Perform 3-dimensional sequential jacobi iteration algorithm
std::vector<double> * jacobi3DPar(const std::vector<double> &bounds, const double epsilon, const unsigned long n, std::vector<double> *in,
                 std::vector<double> *out, int rank, int size, int ghosts);


void setBoundaries(const int dim, const unsigned long n, const std::vector<double> &bounds,
                   std::vector<double> &in);


#endif //PARALLEL_STENCIL_H
