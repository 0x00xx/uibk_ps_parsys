

#include <cassert>
#include <cmath>
#include "Stencil.h"
#include "chrono_timer.h"
#include <omp.h>
#include <iostream>
#include <mpi.h>
#include <unistd.h>

using namespace std;

int count = 0;

// Positions in bounds array
constexpr unsigned long B_L = 0; // left
constexpr unsigned long B_R = 1; // right
constexpr unsigned long B_U = 2; // up
constexpr unsigned long B_D = 3; // down
constexpr unsigned long B_F = 4; // forward
constexpr unsigned long B_B = 5; // backward


void jacobiIter1DSeq(const vector<double> &bounds, const vector<double> &in, vector<double> &out)
{
    for (unsigned long i = 1; i < (in.size() - 1); ++i) {
        out.at(i) = calc1D(in.at(i), in.at(i - 1), in.at(i + 1));
    }
}

void jacobiIter2DSeq(const unsigned long n, const vector<double> &bounds, const vector<double> &in, vector<double> &out)
{
    for (unsigned long i = 1; i < n - 1; ++i) {
        for (unsigned long j = 1; j < n - 1; ++j) {
            out.at(n*i + j) = calc2D(in.at(n*i + j), in.at(n*i + j - 1), in.at(n*i + j + 1), in.at(n*(i - 1) + j), in.at(n*(i + 1) + j));
        }
    }
}


void jacobiIter3DSeq(const unsigned long n, const vector<double> &bounds, const vector<double> &in, vector<double> &out)
{
    for (unsigned long i = 1; i < n - 1; ++i) {
        for (unsigned long j = 1; j < n - 1; ++j) {
            for (unsigned long k = 1; k < n - 1; ++k) {
                out.at(n*n*i + n*j + k) = calc3D(in.at(n*n*i + n*j + k), in.at(n*n*i + n*j + k - 1), in.at(n*n*i + n*j + k + 1),
                                                 in.at(n*n*i + n*(j - 1) + k), in.at(n*n*i + n*(j + 1) + k), in.at(n*n*(i - 1) + n*j + k),
                                                 in.at(n*n*(i + 1) + n*j + k));
            }
        }
    }
}

void jacobi1DSeq(const vector<double> &bounds, const double epsilon, vector<double> *in, vector<double> *out)
{
    assert(in->size() == out->size());
    assert(bounds.size() >= 2);

    do {
        jacobiIter1DSeq(bounds, *in, *out);
        auto tmp = out;
        out = in;
        in = tmp;
    } while (!deltaBelowEpsilon(epsilon, *out, *in));
}

void jacobi2DSeq(const vector<double> &bounds, const double epsilon, const unsigned long n, std::vector<double> *in, std::vector<double> *out)
{
    assert(in->size() == out->size());
    assert(bounds.size() >= 4);

    do {
        jacobiIter2DSeq(n, bounds, *in, *out);
        auto tmp = out;
        out = in;
        in = tmp;
        count++;
    } while (!deltaBelowEpsilon(epsilon, *out, *in));
    std::cout<<count<<std::endl;
}

void
jacobi3DSeq(const std::vector<double> &bounds, const double epsilon, const unsigned long n, std::vector<double> *in, std::vector<double> *out)
{
    assert(in->size() == out->size());
    assert(bounds.size() >= 6);

    do {
        jacobiIter3DSeq(n, bounds, *in, *out);
        auto tmp = out;
        out = in;
        in = tmp;
    } while (!deltaBelowEpsilon(epsilon, *out, *in));
}


void jacobiIter1DPar(const unsigned long n, const vector<double> &bounds, const vector<double> &in, vector<double> &out, int rank, int size,
                     int ghosts, int lowerGhosts, int upperGhosts){
    int extensionSize = 2*ghosts;
    if(rank==0 || rank==size-1)
        extensionSize = ghosts;
//#pragma omp parallel for schedule(static)
    for (unsigned long i = ghosts - lowerGhosts; i < n/size + extensionSize - ghosts + upperGhosts; ++i) {
        out.at(i) = calc1D(in.at(i), in.at(i - 1), in.at(i + 1));
    }
}


void jacobiIter2DPar(const unsigned long n, const vector<double> &bounds, const vector<double> &in, vector<double> &out, int rank, int size,
                     int ghosts, int lowerGhosts, int upperGhosts){
    int extensionSize = 2*ghosts;
    if(rank==0 || rank==size-1)
        extensionSize = ghosts;
//#pragma omp parallel for schedule(static)
    for (unsigned long i = ghosts - lowerGhosts; i < n/size + extensionSize - ghosts + upperGhosts; ++i) {
        for (unsigned long j = 1; j < n - 1; ++j) {
            out.at(n*i + j) = calc2D(in.at(n*i + j), in.at(n*i + j - 1), in.at(n*i + j + 1), in.at(n*(i - 1) + j), in.at(n*(i + 1) + j));
        }
    }
}


void jacobiIter3DPar(const unsigned long n, const vector<double> &bounds, const vector<double> &in, vector<double> &out, int rank, int size,
                     int ghosts, int lowerGhosts, int upperGhosts)
{
    int extensionSize = 2*ghosts;
    if(rank==0 || rank==size-1)
        extensionSize = ghosts;
    //double localDiff = 0;
    //cout << "Iter 3D -> Calc layers:" << (n/size - ghosts + upperGhosts + extensionSize) << "-" << (ghosts - lowerGhosts) <<
    //     " size in " << in.size() << " " << rank << std::endl;
    //#pragma omp parallel for schedule(static)
    for (unsigned long i = ghosts - lowerGhosts; i < n/size + extensionSize - ghosts + upperGhosts; ++i) {
        for (unsigned long j = 1; j < n - 1; ++j) {
            for (unsigned long k = 1; k < n - 1; ++k) {
                //cout << i << j << k << endl;
                out.at(n*n*i + n*j + k) = calc3D(in.at(n*n*i + n*j + k), in.at(n*n*i + n*j + k - 1), in.at(n*n*i + n*j + k + 1),
                                                 in.at(n*n*i + n*(j - 1) + k), in.at(n*n*i + n*(j + 1) + k), in.at(n*n*(i - 1) + n*j + k),
                                                 in.at(n*n*(i + 1) + n*j + k));
                //localDiff += fabs(out.at(n*n*i + n*j + k)-in.at(n*n*i + n*j + k));
            }
        }
    }
    //cout << "END Iter 3D" << std::endl;
    //return localDiff;
}

std::vector<double> * jacobi1DPar(const vector<double> &bounds, const double epsilon, const unsigned long n, std::vector<double> *in, std::vector<double> *out,
                                  int rank, int size, int ghosts){
    assert(in->size() == out->size());
    assert(bounds.size() >= 2);

    int currGhosts = ghosts;
    double localsum;
    double globalsum;
    MPI_Status status;
    do {

        //if changing ghosts are 0 --> exchange the ghosts
        if (currGhosts == 0) {
            //cout << "exchange" << endl;
            MPI_Barrier(MPI_COMM_WORLD);
            //Send up
            if (rank < size - 1)
                MPI_Send(&in->at(in->size() - 2*ghosts), ghosts, MPI_DOUBLE, rank + 1, 2, MPI_COMM_WORLD);
            if (rank > 0)
                MPI_Recv(&in->at(0), ghosts, MPI_DOUBLE, rank - 1, 2, MPI_COMM_WORLD, &status);
            MPI_Barrier(MPI_COMM_WORLD);
            // Send down
            if (rank > 0)
                MPI_Send(&in->at(ghosts), ghosts, MPI_DOUBLE, rank - 1, 1, MPI_COMM_WORLD);
            if (rank < size - 1)
                MPI_Recv(&in->at(in->size() - ghosts), ghosts, MPI_DOUBLE, rank + 1, 1, MPI_COMM_WORLD, &status);
            MPI_Barrier(MPI_COMM_WORLD);

            currGhosts = ghosts;
            std::copy(in->begin(), in->end(), out->begin());
        }
        if (rank == 0) {
            jacobiIter1DPar(n, bounds, *in, *out, rank, size, ghosts, ghosts - 1, currGhosts - 1);
            localsum = deltaPar(*out, *in, 0, ghosts);
        } else if (rank == size - 1) {
            jacobiIter1DPar(n, bounds, *in, *out, rank, size, ghosts, currGhosts - 1, ghosts - 1);
            localsum = deltaPar(*out, *in, ghosts, 0);
        } else {
            jacobiIter1DPar(n, bounds, *in, *out, rank, size, ghosts, currGhosts - 1, currGhosts - 1);
            localsum = deltaPar(*out, *in, ghosts, ghosts);
        }
        currGhosts--;
        auto tmp = out;
        out = in;
        in = tmp;

        //MPI_Reduce(&localsum, &globalsum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        //MPI_Bcast(&globalsum, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        globalsum = 0;
        MPI_Allreduce(&localsum, &globalsum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

        //cout << "GS " << globalsum << " currGhosts: " << currGhosts << endl;
    } while (globalsum > epsilon);
    return in;
}

std::vector<double> * jacobi2DPar(const vector<double> &bounds, const double epsilon, const unsigned long n, std::vector<double> *in, std::vector<double> *out,
                 int rank, int size, int ghosts)
{
    assert(in->size() == out->size());
    assert(in->size()%(n) == 0);
    assert(bounds.size() >= 4);

    int currGhosts = ghosts;
    double localsum;
    double globalsum;
    do {
		count++;
        //if changing ghosts are 0 --> exchange the ghosts
        if (currGhosts == 0) {
            //cout << "exchange" << endl;
            MPI_Barrier(MPI_COMM_WORLD); // TODO is this needed?

            MPI_Request ioToWaitFor[4] = {MPI_REQUEST_NULL, MPI_REQUEST_NULL, MPI_REQUEST_NULL, MPI_REQUEST_NULL};

            //Send up
            if (rank < size - 1)
                MPI_Isend(&in->at(in->size() - 2*ghosts*n), ghosts*n, MPI_DOUBLE,
                         rank + 1, 2, MPI_COMM_WORLD, &ioToWaitFor[0]);
            if (rank > 0)
                MPI_Irecv(&in->at(0), ghosts*n, MPI_DOUBLE, rank - 1, 2, MPI_COMM_WORLD, &ioToWaitFor[1]);
            //MPI_Barrier(MPI_COMM_WORLD); // TODO is this needed?
            // Send down
            if (rank > 0)
                MPI_Isend(&in->at(ghosts*n), ghosts*n, MPI_DOUBLE, rank - 1, 1, MPI_COMM_WORLD, &ioToWaitFor[2]);
            if (rank < size - 1)
                MPI_Irecv(&in->at(in->size() - ghosts*n), ghosts*n, MPI_DOUBLE,
                         rank + 1, 1, MPI_COMM_WORLD, &ioToWaitFor[3]);

            MPI_Waitall(4, ioToWaitFor, MPI_STATUSES_IGNORE);
            MPI_Barrier(MPI_COMM_WORLD); // TODO is this needed?

            currGhosts = ghosts;
            std::copy(in->begin(), in->end(), out->begin());
        }
        if (rank == 0) {
            jacobiIter2DPar(n, bounds, *in, *out, rank, size, ghosts, ghosts-1, currGhosts-1);
            localsum = deltaPar(*out, *in, 0, ghosts*n);
        } else if (rank == size - 1) {
            jacobiIter2DPar(n, bounds, *in, *out, rank, size, ghosts, currGhosts-1, ghosts-1);
            localsum = deltaPar(*out, *in, ghosts*n, 0);
        } else {
            jacobiIter2DPar(n, bounds, *in, *out, rank, size, ghosts, currGhosts-1, currGhosts-1);
            localsum = deltaPar(*out, *in, ghosts*n, ghosts*n);
        }
        currGhosts--;
        auto tmp = out;
        out = in;
        in = tmp;

        //MPI_Reduce(&localsum, &globalsum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        //MPI_Bcast(&globalsum, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        globalsum = 0;
        MPI_Allreduce( &localsum, &globalsum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );

        //cout << "GS " << globalsum << " currGhosts: " << currGhosts << endl;
    } while (globalsum > epsilon);
    
    std::cout<<count<<std::endl;
    return in;
}

std::vector<double> *jacobi3DPar(const std::vector<double> &bounds, const double epsilon, const unsigned long n, std::vector<double> *in, std::vector<double> *out,
            int rank, int size, int ghosts)
{

    assert(in->size() == out->size());
    assert(in->size()%(n*n) == 0);
    assert(bounds.size() >= 6);

    int currGhosts = ghosts;
    double localsum;
    double globalsum;
    MPI_Status status;
    do {

        //if changing ghosts are 0 --> exchange the ghosts
        if (currGhosts == 0) {
            //cout << "exchange" << endl;
            MPI_Barrier(MPI_COMM_WORLD);
            //Send up
            if (rank < size - 1)
                MPI_Send(&in->at(in->size() - 2*ghosts*n*n), ghosts*n*n, MPI_DOUBLE,
                         rank + 1, 2, MPI_COMM_WORLD);
            if (rank > 0)
                MPI_Recv(&in->at(0), ghosts*n*n, MPI_DOUBLE, rank - 1, 2, MPI_COMM_WORLD, &status);
            MPI_Barrier(MPI_COMM_WORLD);
            // Send down
            if (rank > 0)
                MPI_Send(&in->at(ghosts*n*n), ghosts*n*n, MPI_DOUBLE, rank - 1, 1, MPI_COMM_WORLD);
            if (rank < size - 1)
                MPI_Recv(&in->at(in->size() - ghosts*n*n), ghosts*n*n, MPI_DOUBLE,
                         rank + 1, 1, MPI_COMM_WORLD, &status);
            MPI_Barrier(MPI_COMM_WORLD);

            currGhosts = ghosts;
            std::copy(in->begin(), in->end(), out->begin());
        }
        if (rank == 0) {
            jacobiIter3DPar(n, bounds, *in, *out, rank, size, ghosts, ghosts-1, currGhosts-1);
            localsum = deltaPar(*out, *in, 0, ghosts*n*n);
        } else if (rank == size - 1) {
            jacobiIter3DPar(n, bounds, *in, *out, rank, size, ghosts, currGhosts-1, ghosts-1);
            localsum = deltaPar(*out, *in, ghosts*n*n, 0);
        } else {
            jacobiIter3DPar(n, bounds, *in, *out, rank, size, ghosts, currGhosts-1, currGhosts-1);
            localsum = deltaPar(*out, *in, ghosts*n*n, ghosts*n*n);
        }
        currGhosts--;
        auto tmp = out;
        out = in;
        in = tmp;

        //MPI_Reduce(&localsum, &globalsum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        //MPI_Bcast(&globalsum, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        globalsum = 0;
        MPI_Allreduce( &localsum, &globalsum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );

        //cout << "GS " << globalsum << " currGhosts: " << currGhosts << endl;
    } while (globalsum > epsilon);
    return in;
}


void setBoundaries(const int dim, const unsigned long n, const std::vector<double> &bounds, std::vector<double> &in)
{
    switch (dim)
    {
    case 1:
        in.at(0) = bounds.at(B_L);
        in.at(in.size()-1) = bounds.at(B_R);
        break;
    case 2:
        //Breakdown in multiple small loops for memory jump reasons
        for (int i = 0; i < n; ++i) {
            //Top
            in.at(i) = bounds.at(B_U);
        }
        for (int i = 0; i < n; ++i) {
            //Bottom
            in.at(i+(n*n-n)) = bounds.at(B_D);
        }
        for (int i = 0; i < n; ++i) {
            //Left
            in.at(i*n) = bounds.at(B_L);
            //Right
            in.at(i*n+(n-1)) = bounds.at(B_R);
        }
        break;
    case 3:
        // (n*n*n)+(n*n)+(n)
        //TOP
        //std::cout << "TOP Bound" << std::endl;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                in.at((i*n)+(j)) = bounds.at(B_U);
            }
        }
        //BOTTOM
        //std::cout << "BOT Bound" << std::endl;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                in.at((n*n*(n-1))+(i*n)+(j)) = bounds.at(B_D);
            }
        }
        //LEFT RIGHT
        //std::cout << "LR Bound" << std::endl;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                in.at((n*n*i)+(j*n)+(0)) = bounds.at(B_L);
                in.at((n*n*i)+(j*n)+(n-1)) = bounds.at(B_R);
            }
        }

        //Front
        //std::cout << "Front Bound" << std::endl;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                in.at((n*n*i)+(0*n)+(j)) = bounds.at(B_F);
            }
        }
        //Back
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                in.at((n*n*i)+((n-1)*n)+(j)) = bounds.at(B_B);
            }
        }
        break;
    }
}

