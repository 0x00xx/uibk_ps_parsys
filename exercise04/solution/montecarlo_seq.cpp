#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#ifndef N
    #define N 1000
#endif

#define VALUE double

int main(int argc, char** argv) {

  long s = N;
  char* temp;
  if (argc >= 2) {
    s = strtoul(argv[1],&temp,0);
  }

    // calculate random points
    srandom(time(NULL));

  long hit = 0;
  VALUE x;
  VALUE y;
  // Check if it is in circle
  for (int i=0; i<s; i++) {
    x = (VALUE)rand()/RAND_MAX;
    y = (VALUE)rand()/RAND_MAX;
    VALUE temp = pow(x,2)+pow(y,2);
    if(temp <= 1.0){
        hit++;
    }
  }

    // print verification result
    printf("Verification: %1.16f\n", (((float)hit/s)*4));
}

