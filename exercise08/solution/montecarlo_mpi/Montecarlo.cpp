	// g++ montecarlo_par2.cpp -o montecarlo_par -O3 -std=c++11 -Wall -lgmp -fopenmp

#include "Montecarlo.h"


using namespace std;

unsigned int state;

double get_random(){
  static std::random_device rd;
  static std::mt19937 e(rd());
  static std::uniform_real_distribution<> dis(0, 1); // range 0 - 1
  return dis(e);
}

double montecarloSeq(long s, int rank){
  long hit = 0;
  double x;
  double y;
  unsigned int state2 = 324646435 + rank;
  // Check if it is in circle
  for (long i=0; i<s; i++) {	
    x = (double)rand_r(&state2) / RAND_MAX;
    y = (double)rand_r(&state2) / RAND_MAX;
    if(pow(x,2)+pow(y,2) <= 1.0){
      hit++;
    }
  }
  return (double) (hit*4)/s;
}

double montecarloPar(long s, int rank){
  long hit = 0;				
  double x;
  double y;
  long i;
  // Check if it is in circle
  #pragma omp threadprivate(state)
  #pragma omp parallel private(x,y) reduction (+:hit)
  {
    state = 2234 + 3337 * omp_get_thread_num() + rank;
    #pragma omp for
    for (i=0; i<s; i++) {
      x = (double)rand_r(&state) / RAND_MAX;
      y = (double)rand_r(&state) / RAND_MAX;
      if(pow(x,2)+pow(y,2) <= 1.0){
        hit++;
      }
    }
  }
  return (double) (hit*4)/s;
}
