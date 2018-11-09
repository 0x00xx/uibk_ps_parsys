// g++ montecarlo_par2.cpp -o montecarlo_par -O3 -std=c++11 -Wall -lgmp -fopenmp

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <gmp.h>
#include <omp.h>
#include <random>

#ifndef N
	#define N 1000
#endif

#define VALUE double

VALUE get_random()
{
    static std::random_device rd;
    static std::mt19937 e(rd());
    static std::uniform_real_distribution<> dis(0, 1); // rage 0 - 1
    return dis(e);
}

int main(int argc, char** argv) {

  long long s = N;
  char *temp;
  if (argc >= 2) {
    s = strtoul(argv[1],&temp,0)/omp_get_max_threads();
  }

	// calculate random points
	srandom(time(NULL));

  long long hit = 0;
  VALUE x;
  VALUE y;
  // Check if it is in circle
    #pragma omp for
    for (long long i=0; i<s; i++) {
      x = get_random();
      y = get_random();
      VALUE temp = pow(x,2)+pow(y,2);
      if(temp <= 1.0){
        hit++;
      }
    }

    mpf_t hit_big;
    mpf_t s_big;
    mpf_t result;
    mpf_init(hit_big);
    mpf_init(s_big);
    mpf_init2(result,256);
    mpf_set_ui(hit_big,hit);
    mpf_mul_ui(hit_big,hit_big,4);
    mpf_set_ui(s_big,s);
    mpf_div(result,hit_big,s_big);
	gmp_printf("Verification: %.20Ff\n", result);
}