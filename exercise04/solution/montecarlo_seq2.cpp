#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <gmp.h>

#ifndef N
	#define N 1000
#endif

#define VALUE double

int main(int argc, char** argv) {

  long long s = N;
  char *temp;
  if (argc >= 2) {
    s = strtoul(argv[1],&temp,0);
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
  mpz_t hit_big;
  mpz_t s_big;
  mpz_t result;
  mpz_init(hit_big);
  mpz_init(s_big);
  mpz_init(result);
  mpz_init_set_ui(hit_big,hit);
  mpz_init_set_ui(s_big,s);
	mpz_cdiv_q (result,hit_big,s_big);
	gmp_printf("Verification: %1.8f\n", result);
}

