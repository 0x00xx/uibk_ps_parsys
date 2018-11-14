// g++ Main.cpp Montecarlo.h Montecarlo.cpp -fopenmp -Wall -O3 -std=c++11 -o montecarlo"

#include "Montecarlo.h"

int main(int argc, char** argv) {

  long s = 10000;
  char *temp;
  if (argc >= 2) {
    s = strtoul(argv[1],&temp,0);
  }
  if (argc == 3) {
	  double piPar = montecarloPar(s);
	  //gmp_printf("Verification: %.20Ff\n", result);
	  printf("Par: %.12f\n",piPar);
  }else{
	  double piSeq = montecarloSeq(s);
	  //gmp_printf("Verification: %.20Ff\n", result);
	  printf("Seq: %.12f\n",piSeq);
  }
}