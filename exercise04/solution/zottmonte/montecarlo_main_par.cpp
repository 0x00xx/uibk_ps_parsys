// g++ montecarlo_main_par.cpp -o montecarlo_par -O3 -std=c++11 -Wall -lgmp -fopenmp

#include "montecarlo_par.cpp"
#include <iostream>

int main(int argc, char** argv) {

  long long s = 10000;
  char *temp;
  if (argc >= 2) {
    s = strtoul(argv[1],&temp,0);
  }
  double pi = getDouble(s);
  //gmp_printf("Verification: %.20Ff\n", result);
  printf("%.12f\n",pi);
}