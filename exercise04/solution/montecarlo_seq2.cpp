#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "gmp.h"

#ifndef N
	#define N 1000
#endif

#define VALUE float

int main(int argc, char** argv) {

  long long s = N;
  if (argc >= 2) {
    s = atoi(argv[1]);
  }

	// calculate random points
	srandom(time(NULL));

  long long hit = 0;
  VALUE x;
  VALUE y;
  // Check if it is in circle
  for (long long i=0; i<s; i++) {
    x = (VALUE)rand()/RAND_MAX;
    y = (VALUE)rand()/RAND_MAX;
    VALUE temp = pow(x,2)+pow(y,2);
    if(temp <= 1.0){
        hit++;
    }
  }

  mpz_t a;
  mpz_t b;
  mpz_t c;
  mpz_init(a);
  mpz_init(b);
  mpz_init(c);
	// print verification result
	//printf("Verification: %1.8f\n", (((double)hit/s)*4));
  printf("%lu %lu",sizeof(double),sizeof(long long));
}

