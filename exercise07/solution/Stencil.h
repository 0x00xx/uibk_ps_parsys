#ifndef PARALLEL_STENCIL_H
#define PARALLEL_STENCIL_H

#include <vector>

void jacobi1DSeq(const std::vector<int> &bounds, double epsilon, std::vector<double> *in, std::vector<double> *out);

void jacobi2DSeq(const std::vector<int> &bounds, const double epsilon, const unsigned long n, std::vector<double> *in, std::vector<double> *out);

void jacobi3DSeq(const std::vector<int> &bounds, const double epsilon, const unsigned long n, std::vector<double> *in, std::vector<double> *out);

void jacobi1DPar(const std::vector<int> &bounds, double epsilon, std::vector<double> *in, std::vector<double> *out);

void jacobi2DPar(const std::vector<int> &bounds, const double epsilon, const unsigned long n, std::vector<double> *in, std::vector<double> *out);

void jacobi3DPar(const std::vector<int> &bounds, const double epsilon, const unsigned long n, std::vector<double> *in, std::vector<double> *out);

void setBoundaries(const int dim, const unsigned long n, const std::vector<int> &bounds, std::vector<double> &in);

bool deltaBelowEpsilon(double epsilon, const std::vector<double> &before, const std::vector<double> &after);

bool deltaBelowEpsilonPar(double epsilon, const std::vector<double> &before, const std::vector<double> &after);

inline double calc1D(const double x, const double xLeft, const double xRight) {
    return (xLeft+xRight+x)/3;
}

inline double calc2D(const double x, const double xLeft, const double xRight, const double xUp, const double xDown) {
    return (xLeft+xRight+xUp+xDown+x)/5;
}

inline double calc3D(const double x, const double xLeft, const double xRight, const double xUp, const double xDown, const double xFront, const double xBehind) {
    return (xLeft+xRight+xUp+xDown+xFront+xBehind+x)/7;
}

#endif
