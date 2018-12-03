#include <cassert>
#include <cmath>
#include "Stencil.h"
#include "chrono_timer.h"
#include <omp.h>
#include <iostream>
#include <cstdlib>


using namespace std;

// Positions in bounds array
constexpr unsigned long B_L = 0; // left
constexpr unsigned long B_R = 1; // right
constexpr unsigned long B_U = 2; // up
constexpr unsigned long B_D = 3; // down
constexpr unsigned long B_F = 4; // forward
constexpr unsigned long B_B = 5; // backward

void jacobiIter1DSeq(const vector<int> &bounds, const vector<double> &in, vector<double> &out) {
    for (unsigned long i = 1; i < (in.size() - 1); ++i) {
        out.at(i) = calc1D(in.at(i), in.at(i - 1), in.at(i + 1));
    }
}

void jacobiIter2DSeq(const unsigned long n, const vector<int> &bounds, const vector<double> &in, vector<double> &out) {
    for (unsigned long i = 1; i < n - 1; ++i) {
        for (unsigned long j = 1; j < n - 1; ++j) {
            out.at(n*i + j) = calc2D(in.at(n*i + j), in.at(n*i + j - 1), in.at(n*i + j + 1), in.at(n*(i - 1) + j), in.at(n*(i + 1) + j));
        }
    }
}

void jacobiIter3DSeq(const unsigned long n, const vector<int> &bounds, const vector<double> &in, vector<double> &out) {
    for (unsigned long i = 1; i < n - 1; ++i) {
        for (unsigned long j = 1; j < n - 1; ++j) {
            for (unsigned long k = 1; k < n - 1; ++k) {
                out.at(n*n*i + n*j + k) = calc3D(in.at(n*n*i + n*j + k), in.at(n*n*i + n*j + k - 1), in.at(n*n*i + n*j + k + 1), in.at(n*n*i + n*(j-1) + k), in.at(n*n*i + n*(j+1) + k),
                                           in.at(n*n*(i-1) + n*j + k),in.at(n*n*(i+1) + n*j + k));
            }
        }
    }
}

void jacobi1DSeq(const vector<int> &bounds, const double epsilon, vector<double> *in, vector<double> *out) {
    assert(in->size() == out->size());
    assert(bounds.size() >= 2);

    do {
        jacobiIter1DSeq(bounds, *in, *out);
        auto tmp = out;
        out = in;
        in = tmp;
    } while (!deltaBelowEpsilon(epsilon, *out, *in));
}

void jacobi2DSeq(const vector<int> &bounds, const double epsilon, const unsigned long n, std::vector<double> *in, std::vector<double> *out) {
    assert(in->size() == out->size());
    assert(bounds.size() >= 4);

    do {
        jacobiIter2DSeq(n, bounds, *in, *out);
        auto tmp = out;
        out = in;
        in = tmp;
    } while (!deltaBelowEpsilon(epsilon, *out, *in));
}

void jacobi3DSeq(const std::vector<int> &bounds, const double epsilon, const unsigned long n, std::vector<double> *in, std::vector<double> *out) {
    assert(in->size() == out->size());
    assert(bounds.size() >= 6);

    do {
        jacobiIter3DSeq(n, bounds, *in, *out);
        auto tmp = out;
        out = in;
        in = tmp;
    } while (!deltaBelowEpsilon(epsilon, *out, *in));
}

void jacobiIter1DPar(const vector<int> &bounds, const vector<double> &in, vector<double> &out) {
    #pragma omp parallel for schedule(static)
    for (unsigned long i = 1; i < (in.size() - 1); ++i) {
        out.at(i) = calc1D(in.at(i), in.at(i - 1), in.at(i + 1));
    }
}

void jacobiIter2DPar(const unsigned long n, const vector<int> &bounds, const vector<double> &in, vector<double> &out) {
    #pragma omp parallel for schedule(static)
    for (unsigned long i = 1; i < n - 1; ++i) {
        for (unsigned long j = 1; j < n - 1; ++j) {
            out.at(n*i + j) = calc2D(in.at(n*i + j), in.at(n*i + j - 1), in.at(n*i + j + 1), in.at(n*(i - 1) + j), in.at(n*(i + 1) + j));
        }
    }
}

void jacobiIter3DPar(const unsigned long n, const vector<int> &bounds, const vector<double> &in, vector<double> &out) {
    #pragma omp parallel for schedule(static)
    for (unsigned long i = 1; i < n - 1; ++i) {
        for (unsigned long j = 1; j < n - 1; ++j) {
            for (unsigned long k = 1; k < n - 1; ++k) {
                out.at(n*n*i + n*j + k) = calc3D(in.at(n*n*i + n*j + k), in.at(n*n*i + n*j + k - 1), in.at(n*n*i + n*j + k + 1), in.at(n*n*i + n*(j-1) + k), in.at(n*n*i + n*(j+1) + k),
                                                 in.at(n*n*(i-1) + n*j + k),in.at(n*n*(i+1) + n*j + k));
            }
        }
    }
}

void jacobi1DPar(const vector<int> &bounds, const double epsilon, vector<double> *in, vector<double> *out) {
    assert(in->size() == out->size());
    assert(bounds.size() >= 2);
    putenv(const_cast<char *>("OMP_CANCELLATION=true"));

    do {
        jacobiIter1DPar(bounds, *in, *out);
        auto tmp = out;
        out = in;
        in = tmp;
    } while (!deltaBelowEpsilonPar(epsilon, *out, *in));
}

void jacobi2DPar(const vector<int> &bounds, const double epsilon, const unsigned long n, std::vector<double> *in, std::vector<double> *out) {
    assert(in->size() == out->size());
    assert(bounds.size() >= 4);

    putenv(const_cast<char *>("OMP_CANCELLATION=true"));
    do {
        jacobiIter2DPar(n, bounds, *in, *out);
        auto tmp = out;
        out = in;
        in = tmp;
    } while (!deltaBelowEpsilonPar(epsilon, *out, *in));
}

void jacobi3DPar(const std::vector<int> &bounds, const double epsilon, const unsigned long n, std::vector<double>* in, std::vector<double>* out) {
    assert(in->size() == out->size());
    assert(bounds.size() >= 6);
    putenv(const_cast<char *>("OMP_CANCELLATION=true"));

    do {
        jacobiIter3DPar(n, bounds, *in, *out);
        auto tmp = out;
        out = in;
        in = tmp;
    } while (!deltaBelowEpsilonPar(epsilon, *out, *in));
}

void setBoundaries(const int dim, const unsigned long n, const std::vector<int> &bounds, std::vector<double> &in) {
    switch (dim) {
        case 1:
            in.at(0) = bounds.at(B_L);
            in.at(in.size()-1) = bounds.at(B_R);
            break;
        case 2:
            //Breakdown in multiple small loops for memory jump reasons
            for (unsigned long i = 0; i < n; ++i) {
                //Top
                in.at(i) = bounds.at(B_U);
            }
            for (unsigned long i = 0; i < n; ++i) {
                //Bottom
                in.at(i+(n*n-n)) = bounds.at(B_D);
            }
            for (unsigned long i = 0; i < n; ++i) {
                //Left
                in.at(i*n) = bounds.at(B_L);
                //Right
                in.at(i*n+(n-1)) = bounds.at(B_R);
            }
            break;
        case 3:
            // (n*n*n)+(n*n)+(n)
            //TOP
            for (unsigned long i = 0; i < n; ++i) {
                for (unsigned long j = 0; j < n; ++j) {
                       in.at((i*n)+(j)) = bounds.at(B_U);
                }
            }
            //BOTTOM
            for (unsigned long i = 0; i < n; ++i) {
                for (unsigned long j = 0; j < n; ++j) {
                    in.at((n*n*(n-1))+(i*n)+(j)) = bounds.at(B_D);
                }
            }
            //LEFT RIGHT
            for (unsigned long i = 0; i < n; ++i) {
                for (unsigned long j = 0; j < n; ++j) {
                    in.at((n*n*i)+(j*n)+(0)) = bounds.at(B_L);
                    in.at((n*n*i)+(j*n)+(n-1)) = bounds.at(B_R);
                }
            }

            //Front
            for (unsigned long i = 0; i < n; ++i) {
                for (unsigned long j = 0; j < n; ++j) {
                    in.at((n*n*i)+(0*n)+(j)) = bounds.at(B_F);
                }
            }
            //Back
            for (unsigned long i = 0; i < n; ++i) {
                for (unsigned long j = 0; j < n; ++j) {
                    in.at((n*n*i)+((n-1)*n)+(j)) = bounds.at(B_B);
                }
            }
            break;
    }
}

bool deltaBelowEpsilon(const double epsilon, const std::vector<double> &before, const std::vector<double> &after) {
    assert(epsilon > 0);
    long double delta = 0;
    for (unsigned long i = 0; i < before.size(); ++i) {
        delta += fabs(before.at(i) - after.at(i));
    }
    return epsilon>delta;
}

bool deltaBelowEpsilonPar(const double epsilon, const std::vector<double> &before, const std::vector<double> &after) {
    assert(epsilon > 0);
    long double delta = 0;
    #pragma omp parallel for schedule(static) reduction(+:delta)
    for (unsigned long i = 0; i < before.size(); ++i) {
        delta += fabs(before.at(i) - after.at(i));
    }
    return epsilon>delta;
}
