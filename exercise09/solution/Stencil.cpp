

#include <cassert>
#include <cmath>
#include "Stencil.h"
#include "chrono_timer.h"
#include <omp.h>
#include <iostream>
#include <mpi.h>
#include <unistd.h>

using namespace std;


// Positions in bounds array
constexpr unsigned long B_L = 0; // left
constexpr unsigned long B_R = 1; // right
constexpr unsigned long B_U = 2; // up
constexpr unsigned long B_D = 3; // down
constexpr unsigned long B_F = 4; // forward
constexpr unsigned long B_B = 5; // backward




void jacobiIter2DSeq(const unsigned long n, const vector<double> &bounds, const vector<double> &in, vector<double> &out)
{
    for (unsigned long i = 1; i < n - 1; ++i) {
        for (unsigned long j = 1; j < n - 1; ++j) {
            out.at(n*i + j) = calc2D(in.at(n*i + j), in.at(n*i + j - 1), in.at(n*i + j + 1), in.at(n*(i - 1) + j), in.at(n*(i + 1) + j));
        }
    }
}


void jacobiIter2DPar(const unsigned long n, const vector<double> &in, vector<double> &out, int rank, int size){
    for (unsigned long i = 1; i < n - 1; ++i) {
        for (unsigned long j = 1; j < n - 1; ++j) {
            out.at(n*i + j) = calc2D(in.at(n*i + j), in.at(n*i + j - 1), in.at(n*i + j + 1), in.at(n*(i - 1) + j), in.at(n*(i + 1) + j));
        }
    }
}

void jacobiIter2DParNonQuad(const unsigned long n, const unsigned long m, const vector<double> &in, vector<double> &out, int rank, int size){
    for (unsigned long i = 1; i < m - 1; ++i) {
        for (unsigned long j = 1; j < n - 1; ++j) {
            out.at(m*i + j) = calc2D(in.at(m*i + j), in.at(m*i + j - 1), in.at(m*i + j + 1), in.at(m*(i - 1) + j), in.at(m*(i + 1) + j));
        }
    }
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
    } while (!deltaBelowEpsilon(epsilon, *out, *in));
    std::copy(in->begin(), in->end(), out->begin());
}




std::vector<double> * jacobi2DPar(const vector<double> &bounds, const double epsilon, const unsigned long n, std::vector<double> *in, std::vector<double> *out,
                 int rank, int size)
{
    assert(in->size() == out->size());
    assert(in->size()%(n) == 0);
    assert(bounds.size() >= 4);
    /*int blockSize = 66;
    std::vector<double> *blockIn = new std::vector<double>(blockSize*blockSize);
    std::vector<double> *blockOut = new std::vector<double>(blockSize*blockSize);

	//make different blocks based on rank
	for(int i = 0; i< blockSize; i++){
		std::copy(in->begin()+n*i+((n-2)/8)*(rank%8) + ((rank/8)*(n*(n-2))), 
			in->begin()+n*i+((n-2)/8)*(rank%8) + ((rank/8)*(n*(n-2))) + 66, blockIn->begin()+blockSize*i);
	}*/
	int blockSize = 0;
    int blockSizeM = 0;
	int blockSizeN = 0;

	if(size == 4){
		blockSize = (n-2)/2 + 2;
	}else if(size == 16){
		blockSize = (n-2)/4 + 2;
	}else if(size == 64){
		blockSize =(n-2)/8 + 2;
	}else if(size == 2){
        blockSizeN = n;
        blockSizeM = (n-2)/2 + 2; 
    }else if(size == 8){
        blockSizeN = (n-2)/2 + 2; 
        blockSizeM = (n-2)/4 + 2; 
    }else if(size == 32){
        blockSizeN = (n-2)/4 + 2; 
        blockSizeM = (n-2)/8 + 2; 
    }
	 
	//std::cout << blockSize<<std::endl;
    std::vector<double> *blockIn;
    std::vector<double> *blockOut;
    std::vector<double> *bot;
    std::vector<double> *top;
    std::vector<double> *right;
    std::vector<double> *left;

    if(size == 2 || size == 8 || size == 32){
        blockIn = new std::vector<double>(blockSizeN*blockSizeM);
        blockOut = new std::vector<double>(blockSizeN*blockSizeM);
        bot = new std::vector<double>(blockSizeN*blockSizeM);
        right = new std::vector<double>(blockSizeN*blockSizeM);
        top = new std::vector<double>(blockSizeN*blockSizeM);
        left = new std::vector<double>(blockSizeN*blockSizeM);
    }else{
        blockIn = new std::vector<double>(blockSize*blockSize);
        blockOut = new std::vector<double>(blockSize*blockSize);
        bot = new std::vector<double>(blockSize*blockSize);
        right = new std::vector<double>(blockSize*blockSize);
        top = new std::vector<double>(blockSize*blockSize);
        left = new std::vector<double>(blockSize*blockSize);

    }
	
	
	if(size == 4){
		if(rank == 0){
			for(int i = 0; i< blockSize; i++){
				std::copy(in->begin()+n*i, in->begin()+n*i+blockSize, blockIn->begin()+blockSize*i);
			}
		}else if(rank == 1){
			for(int i = 0; i< blockSize; i++){
				std::copy(in->begin()+(blockSize-2)+n*i, in->begin()+(blockSize-2)+n*i+blockSize, blockIn->begin()+blockSize*i);
			}
		}else if(rank == 2){
			for(int i = 0; i< blockSize; i++){
				std::copy(in->begin()+n*i+n*(blockSize-2), in->begin()+n*i+n*(blockSize-2)+blockSize, blockIn->begin()+blockSize*i);
			}
		}else if(rank == 3){
			for(int i = 0; i< blockSize; i++){
				std::copy(in->begin()+n*i+n*(blockSize-2)+blockSize-2, in->begin()+n*i+n*(blockSize-2)+blockSize-2+blockSize, blockIn->begin()+blockSize*i);
			}
		}
	}else if(size == 16){
		
		for(int i = 0; i< blockSize; i++){
			std::copy(in->begin()+n*i+(blockSize-2)*(rank%4) + ((rank/4)*n*(blockSize-2)), 
				in->begin()+n*i+(blockSize-2)*(rank%4) + ((rank/4)*n*(blockSize-2)) + blockSize, blockIn->begin()+blockSize*i);
		}
	}else if(size == 64){
		for(int i = 0; i< blockSize; i++){
			std::copy(in->begin()+n*i+(blockSize-2)*(rank%8) + ((rank/8)*n*(blockSize-2)), 
				in->begin()+n*i+(blockSize-2)*(rank%8) + ((rank/8)*n*(blockSize-2)) + blockSize, blockIn->begin()+blockSize*i);
		}
	}else if(size == 2){
       if(rank == 0){
            for(int i = 0; i< blockSizeM; i++){
                std::copy(in->begin()+n*i, in->begin()+n*i+blockSizeN, blockIn->begin()+blockSizeN*i);
            }
            
       }else if(rank == 1){
            for(int i = 0; i< blockSizeM; i++){
                std::copy(in->begin()+n*i+n*(blockSizeM-2), in->begin()+n*i+n*(blockSizeM-2)+blockSizeN, blockIn->begin()+blockSizeN*i);
            }
            
       }
    }else if(size == 8){
        for(int i = 0; i< blockSizeM; i++){
            std::copy(in->begin()+n*i+(rank%4)*(blockSizeM-2)+(rank/4)*n*(blockSizeN-2), 
                in->begin()+n*i+(rank%4)*(blockSizeM-2)+(rank/4)*n*(blockSizeN-2) + blockSizeN, blockIn->begin()+blockSizeN*i);
        }
        
    }else if(size == 32){
        for(int i = 0; i< blockSizeM; i++){
            std::copy(in->begin()+n*i+(rank%8)*(blockSizeM-2)+(rank/8)*n*(blockSizeN-2), 
                in->begin()+n*i+(rank%8)*(blockSizeM-2)+(rank/8)*n*(blockSizeN-2) + blockSizeN, blockIn->begin()+blockSizeN*i);
        }
    }
	
	std::copy(blockIn->begin(), blockIn->end(), blockOut->begin());
		
    double localsum;
    double globalsum;
	MPI_Status status;
    int count = 0;
    do {
		MPI_Request ioToWaitFor[8] = {MPI_REQUEST_NULL, MPI_REQUEST_NULL, MPI_REQUEST_NULL, MPI_REQUEST_NULL, MPI_REQUEST_NULL,
                                      MPI_REQUEST_NULL, MPI_REQUEST_NULL, MPI_REQUEST_NULL};
		if(count>0){
			if(size == 4){
				if(rank == 0){	//left above
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &ioToWaitFor[0]); 
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, 2, 0, MPI_COMM_WORLD, &ioToWaitFor[1]);
					
					MPI_Irecv(&bot->at(0), blockSize*blockSize, MPI_DOUBLE, 2, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
					MPI_Irecv(&right->at(0), blockSize*blockSize, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
				
				}else if(rank == 1){
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &ioToWaitFor[0]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, 3, 0, MPI_COMM_WORLD, &ioToWaitFor[1]);
					
					MPI_Irecv(&left->at(0), blockSize*blockSize, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
					MPI_Irecv(&bot->at(0), blockSize*blockSize, MPI_DOUBLE, 3, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
				
				}else if(rank == 2){
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, 3, 0, MPI_COMM_WORLD, &ioToWaitFor[0]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &ioToWaitFor[1]);
					
					MPI_Irecv(&right->at(0), blockSize*blockSize, MPI_DOUBLE, 3, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
					MPI_Irecv(&top->at(0), blockSize*blockSize, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
				}else if(rank ==3){
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &ioToWaitFor[0]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, 2, 0, MPI_COMM_WORLD, &ioToWaitFor[1]);
					
					MPI_Irecv(&top->at(0), blockSize*blockSize, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
					MPI_Irecv(&left->at(0), blockSize*blockSize, MPI_DOUBLE, 2, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
				}
				
				MPI_Barrier(MPI_COMM_WORLD);
				
				if(rank == 0){	//left above
					std::copy(bot->begin()+blockSize+1, bot->begin()+blockSize+1+blockSize-1, blockIn->begin()+blockSize*(blockSize-1)+1); //
					for(int i = 0; i<blockSize; i++){
						(*blockIn)[blockSize*i+(blockSize-1)] = right->at(blockSize*i+1);   //
					}
				}else if(rank == 1){
					std::copy(bot->begin()+blockSize, bot->begin()+blockSize+blockSize-1, blockIn->begin()+blockSize*(blockSize-1));
					for(int i = 1; i<blockSize; i++){
						(*blockIn)[blockSize*i] = left->at(blockSize*i+(blockSize-2)); //
					}
				}else if(rank == 2){
					std::copy(top->begin()+blockSize*(blockSize-2)+1, top->begin()+blockSize*(blockSize-2)+blockSize-1, blockIn->begin());
					for(int i = 0; i<blockSize; i++){
						(*blockIn)[blockSize*i+(blockSize-1)] = right->at(blockSize*i+1);  
					}
				}else if(rank ==3){
					std::copy(top->begin()+blockSize*(blockSize-2), top->begin()+blockSize*(blockSize-2)+blockSize, blockIn->begin());
					for(int i = 1; i<blockSize; i++){
						(*blockIn)[blockSize*i] = left->at(blockSize*i+(blockSize-2)); 
					}
				}
			}else if(size == 16) {
				if(rank == 0){	//left above
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[0]); 
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 4, 0, MPI_COMM_WORLD, &ioToWaitFor[1]);
					
					MPI_Irecv(&bot->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 4, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
					MPI_Irecv(&right->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
				}else if(rank == 3){	//right above
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, &ioToWaitFor[0]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank+4, 0, MPI_COMM_WORLD, &ioToWaitFor[1]);
					
					MPI_Irecv(&left->at(0), blockSize*blockSize, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
					MPI_Irecv(&bot->at(0), blockSize*blockSize, MPI_DOUBLE, rank +4, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
				}else if(rank == 15){	//right down
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank-4, 0, MPI_COMM_WORLD, &ioToWaitFor[0]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, &ioToWaitFor[1]);
					
					MPI_Irecv(&top->at(0), blockSize*blockSize, MPI_DOUBLE, rank-4, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
					MPI_Irecv(&left->at(0), blockSize*blockSize, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
				}else if(rank == 12){	//left down
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD, &ioToWaitFor[0]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank-4, 0, MPI_COMM_WORLD, &ioToWaitFor[1]);
					
					MPI_Irecv(&right->at(0), blockSize*blockSize, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
					MPI_Irecv(&top->at(0), blockSize*blockSize, MPI_DOUBLE, rank-4, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
				}else if(rank >12){	//bot
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &ioToWaitFor[0]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[1]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 4, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
					
					MPI_Irecv(&left->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
					MPI_Irecv(&top->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 4, 0, MPI_COMM_WORLD, &ioToWaitFor[4]);
					MPI_Irecv(&right->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[5]);
				}else if(rank >0 && rank < 3){	//top				
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &ioToWaitFor[0]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[1]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 4, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
					
					MPI_Irecv(&left->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
					MPI_Irecv(&bot->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 4, 0, MPI_COMM_WORLD, &ioToWaitFor[4]);
					MPI_Irecv(&right->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[5]);
				}else if(rank % 4 == 0){	//left
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 4, 0, MPI_COMM_WORLD, &ioToWaitFor[0]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[1]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 4, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
					
					MPI_Irecv(&bot->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 4, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
					MPI_Irecv(&top->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 4, 0, MPI_COMM_WORLD, &ioToWaitFor[4]);
					MPI_Irecv(&right->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[5]);		
				}else if(rank % 4 == 3){	//right
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &ioToWaitFor[0]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 4, 0, MPI_COMM_WORLD, &ioToWaitFor[1]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 4, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
					
					MPI_Irecv(&left->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
					MPI_Irecv(&top->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 4, 0, MPI_COMM_WORLD, &ioToWaitFor[4]);
					MPI_Irecv(&bot->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 4, 0, MPI_COMM_WORLD, &ioToWaitFor[5]);
				}else {	//middle
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &ioToWaitFor[0]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 4, 0, MPI_COMM_WORLD, &ioToWaitFor[1]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 4, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
					
					MPI_Irecv(&bot->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 4, 0, MPI_COMM_WORLD, &ioToWaitFor[4]);
					MPI_Irecv(&top->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 4, 0, MPI_COMM_WORLD, &ioToWaitFor[5]);
					MPI_Irecv(&right->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[6]);
					MPI_Irecv(&left->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &ioToWaitFor[7]);
				}
				
				MPI_Barrier(MPI_COMM_WORLD);
				
				if(rank == 0){	//left top
					std::copy(bot->begin()+blockSize+1, bot->begin()+blockSize+1+blockSize-1, blockIn->begin()+blockSize*(blockSize-1)+1); 
					for(int i = 0; i<blockSize; i++){
						(*blockIn)[blockSize*i+(blockSize-1)] = right->at(blockSize*i+1);   
					}
				}else if(rank == 3){
					std::copy(bot->begin()+blockSize, bot->begin()+blockSize+blockSize-1, blockIn->begin()+blockSize*(blockSize-1));
					for(int i = 1; i<blockSize; i++){
						(*blockIn)[blockSize*i] = left->at(blockSize*i+(blockSize-2)); //
					}
				}else if(rank == 12){
					std::copy(top->begin()+blockSize*(blockSize-2)+1, top->begin()+blockSize*(blockSize-2)+blockSize-1, blockIn->begin()+1);
					for(int i = 0; i<blockSize; i++){
						(*blockIn)[blockSize*i+(blockSize-1)] = right->at(blockSize*i+1);  
					}
				}else if (rank == 15){
					std::copy(top->begin()+blockSize*(blockSize-2), top->begin()+blockSize*(blockSize-2)+blockSize, blockIn->begin());
					for(int i = 1; i<blockSize; i++){
						(*blockIn)[blockSize*i] = left->at(blockSize*i+(blockSize-2)); 
					}
				}else if(rank >0 && rank < 3){	//top	
					std::copy(bot->begin()+blockSize+1, bot->begin()+blockSize+1+blockSize-1, blockIn->begin()+blockSize*(blockSize-1)+1); 
					for(int i = 0; i<blockSize; i++){
						(*blockIn)[blockSize*i] = left->at(blockSize*i+(blockSize-2)); 
						(*blockIn)[blockSize*i+(blockSize-1)] = right->at(blockSize*i+1);
					}	
				}else if(rank > 12){
					std::copy(top->begin()+blockSize*(blockSize-2), top->begin()+blockSize*(blockSize-2)+blockSize, blockIn->begin());
					for(int i = 0; i<blockSize; i++){
						(*blockIn)[blockSize*i] = left->at(blockSize*i+(blockSize-2)); 
						(*blockIn)[blockSize*i+(blockSize-1)] = right->at(blockSize*i+1);
					}
				}else if(rank % 4 == 0){	//left
					std::copy(bot->begin()+blockSize+1, bot->begin()+blockSize+1+blockSize-1, blockIn->begin()+blockSize*(blockSize-1)+1); 
					std::copy(top->begin()+blockSize*(blockSize-2), top->begin()+blockSize*(blockSize-2)+blockSize, blockIn->begin());
					for(int i = 0; i<blockSize; i++){ 
						(*blockIn)[blockSize*i+(blockSize-1)] = right->at(blockSize*i+1);
					}		
				}else if(rank % 4 == 3){	//right
					std::copy(bot->begin()+blockSize+1, bot->begin()+blockSize+1+blockSize-1, blockIn->begin()+blockSize*(blockSize-1)+1); 
					std::copy(top->begin()+blockSize*(blockSize-2), top->begin()+blockSize*(blockSize-2)+blockSize, blockIn->begin());
					for(int i = 0; i<blockSize; i++){
						(*blockIn)[blockSize*i] = left->at(blockSize*i+(blockSize-2)); 
					}
				}else {	//middle
					std::copy(bot->begin()+blockSize+1, bot->begin()+blockSize+1+blockSize-1, blockIn->begin()+blockSize*(blockSize-1)+1); 
					std::copy(top->begin()+blockSize*(blockSize-2), top->begin()+blockSize*(blockSize-2)+blockSize, blockIn->begin());
					for(int i = 0; i<blockSize; i++){
						(*blockIn)[blockSize*i] = left->at(blockSize*i+(blockSize-2)); 
						(*blockIn)[blockSize*i+(blockSize-1)] = right->at(blockSize*i+1);
					}
				}
			}else if(size == 64) {
				if(rank == 0){	//left above
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[0]); 
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 8, 0, MPI_COMM_WORLD, &ioToWaitFor[1]);
					
					MPI_Irecv(&bot->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 8, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
					MPI_Irecv(&right->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
				}else if(rank == 7){	//right above
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, &ioToWaitFor[0]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank+8, 0, MPI_COMM_WORLD, &ioToWaitFor[1]);
					
					MPI_Irecv(&left->at(0), blockSize*blockSize, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
					MPI_Irecv(&bot->at(0), blockSize*blockSize, MPI_DOUBLE, rank +8, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
				}else if(rank == 63){	//right down
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank-8, 0, MPI_COMM_WORLD, &ioToWaitFor[0]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, &ioToWaitFor[1]);
					
					MPI_Irecv(&top->at(0), blockSize*blockSize, MPI_DOUBLE, rank-8, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
					MPI_Irecv(&left->at(0), blockSize*blockSize, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
				}else if(rank == 56){	//left down
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD, &ioToWaitFor[0]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank-8, 0, MPI_COMM_WORLD, &ioToWaitFor[1]);
					
					MPI_Irecv(&right->at(0), blockSize*blockSize, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
					MPI_Irecv(&top->at(0), blockSize*blockSize, MPI_DOUBLE, rank-8, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
				}else if(rank >56){	//bot
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &ioToWaitFor[0]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[1]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 8, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
					
					MPI_Irecv(&left->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
					MPI_Irecv(&top->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 8, 0, MPI_COMM_WORLD, &ioToWaitFor[4]);
					MPI_Irecv(&right->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[5]);
				}else if(rank >0 && rank < 7){	//top				
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &ioToWaitFor[0]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[1]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 8, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
					
					MPI_Irecv(&left->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
					MPI_Irecv(&bot->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 8, 0, MPI_COMM_WORLD, &ioToWaitFor[4]);
					MPI_Irecv(&right->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[5]);
				}else if(rank % 8 == 0){	//left
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 8, 0, MPI_COMM_WORLD, &ioToWaitFor[0]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[1]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 8, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
					
					MPI_Irecv(&bot->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 8, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
					MPI_Irecv(&top->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 8, 0, MPI_COMM_WORLD, &ioToWaitFor[4]);
					MPI_Irecv(&right->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[5]);		
				}else if(rank % 8 == 7){	//right
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &ioToWaitFor[0]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 8, 0, MPI_COMM_WORLD, &ioToWaitFor[1]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 8, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
					
					MPI_Irecv(&left->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
					MPI_Irecv(&top->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 8, 0, MPI_COMM_WORLD, &ioToWaitFor[4]);
					MPI_Irecv(&bot->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 8, 0, MPI_COMM_WORLD, &ioToWaitFor[5]);
				}else {	//middle
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &ioToWaitFor[0]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 8, 0, MPI_COMM_WORLD, &ioToWaitFor[1]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 8, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
					MPI_Isend(&blockIn->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
					
					MPI_Irecv(&bot->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 8, 0, MPI_COMM_WORLD, &ioToWaitFor[4]);
					MPI_Irecv(&top->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 8, 0, MPI_COMM_WORLD, &ioToWaitFor[5]);
					MPI_Irecv(&right->at(0), blockSize*blockSize, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[6]);
					MPI_Irecv(&left->at(0), blockSize*blockSize, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &ioToWaitFor[7]);
				}
				
				MPI_Barrier(MPI_COMM_WORLD);
				
				if(rank == 0){	//left top
					std::copy(bot->begin()+blockSize+1, bot->begin()+blockSize+1+blockSize-1, blockIn->begin()+blockSize*(blockSize-1)+1); 
					for(int i = 0; i<blockSize; i++){
						(*blockIn)[blockSize*i+(blockSize-1)] = right->at(blockSize*i+1);   
					}
				}else if(rank == 7){
					std::copy(bot->begin()+blockSize, bot->begin()+blockSize+blockSize-1, blockIn->begin()+blockSize*(blockSize-1));
					for(int i = 1; i<blockSize; i++){
						(*blockIn)[blockSize*i] = left->at(blockSize*i+(blockSize-2)); 
					}
				}else if(rank == 56){
					std::copy(top->begin()+blockSize*(blockSize-2)+1, top->begin()+blockSize*(blockSize-2)+blockSize-1, blockIn->begin()+1);
					for(int i = 0; i<blockSize; i++){
						(*blockIn)[blockSize*i+(blockSize-1)] = right->at(blockSize*i+1);  
					}
				}else if (rank == 63){
					std::copy(top->begin()+blockSize*(blockSize-2), top->begin()+blockSize*(blockSize-2)+blockSize, blockIn->begin());
					for(int i = 1; i<blockSize; i++){
						(*blockIn)[blockSize*i] = left->at(blockSize*i+(blockSize-2)); 
					}
				}else if(rank >0 && rank < 7){	//top	
					std::copy(bot->begin()+blockSize+1, bot->begin()+blockSize+1+blockSize-1, blockIn->begin()+blockSize*(blockSize-1)+1); 
					for(int i = 0; i<blockSize; i++){
						(*blockIn)[blockSize*i] = left->at(blockSize*i+(blockSize-2)); 
						(*blockIn)[blockSize*i+(blockSize-1)] = right->at(blockSize*i+1);
					}	
				}else if(rank > 56){
					std::copy(top->begin()+blockSize*(blockSize-2), top->begin()+blockSize*(blockSize-2)+blockSize, blockIn->begin());
					for(int i = 0; i<blockSize; i++){
						(*blockIn)[blockSize*i] = left->at(blockSize*i+(blockSize-2)); 
						(*blockIn)[blockSize*i+(blockSize-1)] = right->at(blockSize*i+1);
					}
				}else if(rank % 8 == 0){	//left
					std::copy(bot->begin()+blockSize+1, bot->begin()+blockSize+1+blockSize-1, blockIn->begin()+blockSize*(blockSize-1)+1); 
					std::copy(top->begin()+blockSize*(blockSize-2), top->begin()+blockSize*(blockSize-2)+blockSize, blockIn->begin());
					for(int i = 0; i<blockSize; i++){ 
						(*blockIn)[blockSize*i+(blockSize-1)] = right->at(blockSize*i+1);
					}		
				}else if(rank % 8 == 7){	//right
					std::copy(bot->begin()+blockSize+1, bot->begin()+blockSize+1+blockSize-1, blockIn->begin()+blockSize*(blockSize-1)+1); 
					std::copy(top->begin()+blockSize*(blockSize-2), top->begin()+blockSize*(blockSize-2)+blockSize, blockIn->begin());
					for(int i = 0; i<blockSize; i++){
						(*blockIn)[blockSize*i] = left->at(blockSize*i+(blockSize-2)); 
					}
				}else {	//middle
					std::copy(bot->begin()+blockSize+1, bot->begin()+blockSize+1+blockSize-1, blockIn->begin()+blockSize*(blockSize-1)+1); 
					std::copy(top->begin()+blockSize*(blockSize-2), top->begin()+blockSize*(blockSize-2)+blockSize, blockIn->begin());
					for(int i = 0; i<blockSize; i++){
						(*blockIn)[blockSize*i] = left->at(blockSize*i+(blockSize-2)); 
						(*blockIn)[blockSize*i+(blockSize-1)] = right->at(blockSize*i+1);
					}
				}
			}else if(size == 2){
                if(rank == 0){ 
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &ioToWaitFor[0]); 
                    
                    MPI_Irecv(&bot->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &ioToWaitFor[1]);
                
                }else if(rank == 1){
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &ioToWaitFor[0]);
                    
                    MPI_Irecv(&top->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &ioToWaitFor[1]);
                }

                MPI_Barrier(MPI_COMM_WORLD);
                
                if(rank == 0){  //left above
                    std::copy(bot->begin()+blockSizeN+1, bot->begin()+blockSizeN+1+blockSizeN-1, blockIn->begin()+blockSizeN*(blockSizeM-1)+1); //
                }else if(rank == 1){
                    std::copy(top->begin()+blockSizeN*(blockSizeM-2)+1, top->begin()+blockSizeN*(blockSizeM-2)+blockSizeN-1, blockIn->begin()+blockSizeN*(blockSizeM-1)+1);
                }
            }else if(size == 8){
                if(rank == 0){  //left above
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &ioToWaitFor[0]); 
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, 2, 0, MPI_COMM_WORLD, &ioToWaitFor[1]); 
                    
                    MPI_Irecv(&right->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
                    MPI_Irecv(&bot->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, 2, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
                
                }else if(rank == 1){
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &ioToWaitFor[0]); 
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, 3, 0, MPI_COMM_WORLD, &ioToWaitFor[1]); 
                    
                    MPI_Irecv(&left->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
                    MPI_Irecv(&bot->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, 3, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
                }else if(rank == 6){
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, 4, 0, MPI_COMM_WORLD, &ioToWaitFor[0]); 
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, 7, 0, MPI_COMM_WORLD, &ioToWaitFor[1]); 
                    
                    MPI_Irecv(&top->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, 4, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
                    MPI_Irecv(&right->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, 7, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
                }else if(rank == 7){
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, 5, 0, MPI_COMM_WORLD, &ioToWaitFor[0]); 
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, 6, 0, MPI_COMM_WORLD, &ioToWaitFor[1]); 
                    
                    MPI_Irecv(&top->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, 5, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
                    MPI_Irecv(&left->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, 6, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
                }else if(rank == 2 || rank == 4){
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank - 2, 0, MPI_COMM_WORLD, &ioToWaitFor[0]); 
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[1]); 
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank + 2, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
                    
                    MPI_Irecv(&top->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank - 2, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
                    MPI_Irecv(&right->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[4]);
                    MPI_Irecv(&bot->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank + 2, 0, MPI_COMM_WORLD, &ioToWaitFor[5]);
                }else if(rank == 3 || rank == 5){
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &ioToWaitFor[0]); 
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank + 2, 0, MPI_COMM_WORLD, &ioToWaitFor[1]); 
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank - 2, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
                    
                    MPI_Irecv(&left->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
                    MPI_Irecv(&bot->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank + 2, 0, MPI_COMM_WORLD, &ioToWaitFor[4]);
                    MPI_Irecv(&top->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank - 2, 0, MPI_COMM_WORLD, &ioToWaitFor[5]);
                }
                
                MPI_Barrier(MPI_COMM_WORLD);
                
                if(rank == 0){  //left above
                    std::copy(bot->begin()+blockSizeN+1, bot->begin()+blockSizeN+1+blockSizeN-1, blockIn->begin()+blockSizeN*(blockSizeM-1)+1); 
					for(int i = 0; i<blockSize; i++){
						(*blockIn)[blockSizeN*i+(blockSizeM-1)] = right->at(blockSizeN*i+1);
					}
                }else if(rank == 1){
                    std::copy(bot->begin()+blockSizeN+1, bot->begin()+blockSizeN+1+blockSizeN-1, blockIn->begin()+blockSizeN*(blockSizeM-1)+1); 
					for(int i = 0; i<blockSize; i++){
						(*blockIn)[blockSizeN*i] = left->at(blockSizeN*i+(blockSizeM-2)); 
					}
				}else if(rank == 6){
					std::copy(top->begin()+blockSizeN*(blockSizeM-2), top->begin()+blockSizeN*(blockSizeM-2)+blockSizeN, blockIn->begin());
					for(int i = 0; i<blockSize; i++){
						(*blockIn)[blockSizeN*i+(blockSizeM-1)] = right->at(blockSizeN*i+1);
					}
				}else if(rank == 7){
                    std::copy(top->begin()+blockSizeN*(blockSizeM-2), top->begin()+blockSizeN*(blockSizeM-2)+blockSizeN, blockIn->begin());
					for(int i = 0; i<blockSize; i++){
						(*blockIn)[blockSizeN*i] = left->at(blockSizeN*i+(blockSizeM-2)); 
					}
				}else if(rank == 2 || rank == 4){
                    std::copy(bot->begin()+blockSizeN+1, bot->begin()+blockSizeN+1+blockSizeN-1, blockIn->begin()+blockSizeN*(blockSizeM-1)+1); 
					std::copy(top->begin()+blockSizeN*(blockSizeM-2), top->begin()+blockSizeN*(blockSizeM-2)+blockSizeN, blockIn->begin());
					for(int i = 0; i<blockSize; i++){
						(*blockIn)[blockSizeN*i+(blockSizeM-1)] = right->at(blockSizeN*i+1);
					}
				}else if(rank == 3 || rank == 5){
                    std::copy(bot->begin()+blockSizeN+1, bot->begin()+blockSizeN+1+blockSizeN-1, blockIn->begin()+blockSizeN*(blockSizeM-1)+1); 
					std::copy(top->begin()+blockSizeN*(blockSizeM-2), top->begin()+blockSizeN*(blockSizeM-2)+blockSizeN, blockIn->begin());
					for(int i = 0; i<blockSize; i++){
						(*blockIn)[blockSizeN*i] = left->at(blockSizeN*i+(blockSizeM-2)); 
					}
				}
               
			}else if(size == 32){
                if(rank == 0){  //left above
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank +1, 0, MPI_COMM_WORLD, &ioToWaitFor[0]); 
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank + 4, 0, MPI_COMM_WORLD, &ioToWaitFor[1]); 
                    
                    MPI_Irecv(&right->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
                    MPI_Irecv(&bot->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank + 4, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
                
                }else if(rank == 3){
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank -1, 0, MPI_COMM_WORLD, &ioToWaitFor[0]); 
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank +4, 0, MPI_COMM_WORLD, &ioToWaitFor[1]); 
                    
                    MPI_Irecv(&left->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank -1, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
                    MPI_Irecv(&bot->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank + 4, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
                }else if(rank == 28){
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank -4, 0, MPI_COMM_WORLD, &ioToWaitFor[0]); 
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank +1, 0, MPI_COMM_WORLD, &ioToWaitFor[1]); 
                    
                    MPI_Irecv(&top->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank -4, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
                    MPI_Irecv(&right->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank +1, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
                }else if(rank == 31){
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank-4, 0, MPI_COMM_WORLD, &ioToWaitFor[0]); 
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, &ioToWaitFor[1]); 
                    
                    MPI_Irecv(&top->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank-4, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
                    MPI_Irecv(&left->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
                }else if(rank < 3){
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &ioToWaitFor[0]); 
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[1]); 
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank + 4, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
                    
                    MPI_Irecv(&left->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
                    MPI_Irecv(&right->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[4]);
                    MPI_Irecv(&bot->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank + 4, 0, MPI_COMM_WORLD, &ioToWaitFor[5]);
                }else if(rank > 28){
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &ioToWaitFor[0]); 
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[1]); 
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank - 4, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
                    
                    MPI_Irecv(&left->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
                    MPI_Irecv(&right->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[4]);
                    MPI_Irecv(&top->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank - 4, 0, MPI_COMM_WORLD, &ioToWaitFor[5]);
                } else if(rank % 4 == 0){	//left
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank - 4, 0, MPI_COMM_WORLD, &ioToWaitFor[0]); 
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[1]); 
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank + 4, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
                    
                    MPI_Irecv(&top->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank - 4, 0, MPI_COMM_WORLD, &ioToWaitFor[5]);
					MPI_Irecv(&right->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[4]);
                    MPI_Irecv(&bot->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank + 4, 0, MPI_COMM_WORLD, &ioToWaitFor[5]);
                }else if(rank % 4 == 3){ 	//right
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &ioToWaitFor[0]); 
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank + 4, 0, MPI_COMM_WORLD, &ioToWaitFor[1]); 
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank - 4, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
                    
                    MPI_Irecv(&left->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
                    MPI_Irecv(&bot->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank + 4, 0, MPI_COMM_WORLD, &ioToWaitFor[5]);
					MPI_Irecv(&top->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank - 4, 0, MPI_COMM_WORLD, &ioToWaitFor[5]);
                } else{
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &ioToWaitFor[0]); 
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[1]); 
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank - 4, 0, MPI_COMM_WORLD, &ioToWaitFor[2]);
                    MPI_Isend(&blockIn->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank + 4, 0, MPI_COMM_WORLD, &ioToWaitFor[3]);
                    
                    MPI_Irecv(&left->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &ioToWaitFor[4]);
                    MPI_Irecv(&right->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &ioToWaitFor[5]);
                    MPI_Irecv(&top->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank - 4, 0, MPI_COMM_WORLD, &ioToWaitFor[6]);
                    MPI_Irecv(&bot->at(0), blockSizeN*blockSizeM, MPI_DOUBLE, rank + 4, 0, MPI_COMM_WORLD, &ioToWaitFor[7]);
                }
                
                MPI_Barrier(MPI_COMM_WORLD);
                
                if(rank == 0){  //left above
                    std::copy(bot->begin()+blockSizeN+1, bot->begin()+blockSizeN+1+blockSizeN-1, blockIn->begin()+blockSizeN*(blockSizeM-1)+1); 
					for(int i = 0; i<blockSize; i++){
						(*blockIn)[blockSizeN*i+(blockSizeM-1)] = right->at(blockSizeN*i+1);
					}
                }else if(rank == 3){
                    std::copy(bot->begin()+blockSizeN+1, bot->begin()+blockSizeN+1+blockSizeN-1, blockIn->begin()+blockSizeN*(blockSizeM-1)+1); 
					for(int i = 0; i<blockSize; i++){
						(*blockIn)[blockSizeN*i] = left->at(blockSizeN*i+(blockSizeM-2)); 
					}
				}else if(rank == 28){
                    std::copy(top->begin()+blockSizeN*(blockSizeM-2), top->begin()+blockSizeN*(blockSizeM-2)+blockSizeN, blockIn->begin());
					for(int i = 0; i<blockSize; i++){
						(*blockIn)[blockSizeN*i+(blockSizeM-1)] = right->at(blockSizeN*i+1);
					}
				}else if(rank == 31){
                    std::copy(top->begin()+blockSizeN*(blockSizeM-2), top->begin()+blockSizeN*(blockSizeM-2)+blockSizeN, blockIn->begin());
					for(int i = 0; i<blockSize; i++){
						(*blockIn)[blockSizeN*i] = left->at(blockSizeN*i+(blockSizeM-2)); 
					}
				}else if(rank < 3){
                    std::copy(bot->begin()+blockSizeN+1, bot->begin()+blockSizeN+1+blockSizeN-1, blockIn->begin()+blockSizeN*(blockSizeM-1)+1); 
					for(int i = 0; i<blockSize; i++){
						(*blockIn)[blockSizeN*i+(blockSizeM-1)] = right->at(blockSizeN*i+1);
						(*blockIn)[blockSizeN*i] = left->at(blockSizeN*i+(blockSizeM-2)); 
					}
				}else if(rank > 28){
                    std::copy(top->begin()+blockSizeN*(blockSizeM-2), top->begin()+blockSizeN*(blockSizeM-2)+blockSizeN, blockIn->begin());
					for(int i = 0; i<blockSize; i++){
						(*blockIn)[blockSizeN*i+(blockSizeM-1)] = right->at(blockSizeN*i+1);
						(*blockIn)[blockSizeN*i] = left->at(blockSizeN*i+(blockSizeM-2)); 
					}
				} else if(rank % 4 == 0){	//left
                    std::copy(bot->begin()+blockSizeN+1, bot->begin()+blockSizeN+1+blockSizeN-1, blockIn->begin()+blockSizeN*(blockSizeM-1)+1); 
					std::copy(top->begin()+blockSizeN*(blockSizeM-2), top->begin()+blockSizeN*(blockSizeM-2)+blockSizeN, blockIn->begin());
					for(int i = 0; i<blockSize; i++){
						(*blockIn)[blockSizeN*i+(blockSizeM-1)] = right->at(blockSizeN*i+1);
					}
				}else if(rank % 4 == 3){ 	//right
                    std::copy(bot->begin()+blockSizeN+1, bot->begin()+blockSizeN+1+blockSizeN-1, blockIn->begin()+blockSizeN*(blockSizeM-1)+1); 
					std::copy(top->begin()+blockSizeN*(blockSizeM-2), top->begin()+blockSizeN*(blockSizeM-2)+blockSizeN, blockIn->begin());
					for(int i = 0; i<blockSize; i++){
						(*blockIn)[blockSizeN*i] = left->at(blockSizeN*i+(blockSizeM-2)); 
					}
				} else{
					std::copy(bot->begin()+blockSizeN+1, bot->begin()+blockSizeN+1+blockSizeN-1, blockIn->begin()+blockSizeN*(blockSizeM-1)+1); 
					std::copy(top->begin()+blockSizeN*(blockSizeM-2), top->begin()+blockSizeN*(blockSizeM-2)+blockSizeN, blockIn->begin());
					for(int i = 0; i<blockSize; i++){
						(*blockIn)[blockSizeN*i+(blockSizeM-1)] = right->at(blockSizeN*i+1);
						(*blockIn)[blockSizeN*i] = left->at(blockSizeN*i+(blockSizeM-2)); 
					}
				}
            }
				
			//MPI_Barrier(MPI_COMM_WORLD);
			
			
			
			//return out;
		}
		//MPI_Barrier(MPI_COMM_WORLD);
        /*for(int i = 0; i<blockSizeM; i++){
			for(int j = 0; j<blockSizeN; j++){
				std::cout<<blockOut->at(i*blockSizeN + j)<< " ";
			}
			std::cout << std::endl;
		}*/
        if(size == 2 || size == 8 || size == 32){   
            jacobiIter2DParNonQuad(blockSizeN, blockSizeM, *blockIn, *blockOut, rank, size);
        }else{
            jacobiIter2DPar(blockSize, *blockIn, *blockOut, rank, size);
        }
		localsum = deltaPar(*blockOut, *blockIn);
        //cout << "LS " << localsum << "  "<<rank << endl;
        
        
		
        auto tmp = blockOut;
        blockOut = blockIn;
        blockIn = tmp;
		
			
        
        globalsum = 0;
        //MPI_Barrier(MPI_COMM_WORLD);
        MPI_Allreduce( &localsum, &globalsum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );
        //MPI_Barrier(MPI_COMM_WORLD);
       

		count++;
		
        //cout << "GS " << globalsum << "  "<<count << endl;
    } while (globalsum > epsilon);
    
    //cout << "GS " << globalsum << " currGhosts: " << currGhosts << endl;
    /*if(rank == 0){
		std::cout<<count<<std::endl;
   
	}*/
	
    
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

