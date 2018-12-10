// mpic++ Main.cpp Montecarlo.h Montecarlo.cpp -fopenmp -Wall -O3 -std=c++11 -o montecarlo
// mpirun -np 4 ./montecarlo 100000000 par

#include "Montecarlo.h"
#include <mpi.h>

int main(int argc, char** argv) {

  long s = 10000;
  char *temp;
  if (argc >= 2) {
    s = strtoul(argv[1],&temp,0);
  }
  
  int rank, size = 0;
  double result = 0.0;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if(size >= 2){
	  size--;
  }
  
  if(rank == 0 && size != 0){
	  double *value = new double[size];
	  MPI_Recv(value, size, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  for(int i = 0; i<size; i++){
		  result += value[i];
	  }
	  std::cout << result << std::endl;
	  free(value);
  }else if (argc == 3) {
	  double value = montecarloPar(s/size, rank);
	  MPI_Send(&value, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	  //gmp_printf("Verification: %.20Ff\n", result);
  }else{
	  double value = montecarloSeq(s/size, rank);
	  MPI_Send(&value, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	  //gmp_printf("Verification: %.20Ff\n", result);
  }
  
  MPI_Finalize();

}
