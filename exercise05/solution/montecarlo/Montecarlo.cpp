// g++ montecarlo_par2.cpp -o montecarlo_par -O3 -std=c++11 -Wall -lgmp -fopenmp

#include "Montecarlo.h"

using namespace std;

double getDoubleSeq(long s);
double getDoublePar(long s);

double get_random(){
  static std::random_device rd;
  static std::mt19937 e(rd());
  static std::uniform_real_distribution<> dis(0, 1); // range 0 - 1
  return dis(e);
}

double montecarloSeq(long s)
{
  long hit = 0;
  double x;
  double y;
  // Check if it is in circle
  for (long i=0; i<s; i++) {
    x = get_random();
    y = get_random();
    double temp = pow(x,2)+pow(y,2);
    if(temp <= 1.0){
      hit++;
    }
  }
  return (double) (hit*4)/s;
}

double montecarloPar(long s){
  long hit = 0;
  double x;
  double y;
  double temp;
  long i;
  // Check if it is in circle
  #pragma omp parallel private(temp, x, y, i) reduction (+:hit)
  {
    x = 0.0;
    y = 0.0;
    temp = 0.0;
    #pragma omp parallel for
    for (i=0; i<s/omp_get_max_threads(); i++) {
      x = get_random();
      y = get_random();
      temp = pow(x,2)+pow(y,2);
      if(temp <= 1.0){
        hit++;
      }
    }
  }
  for(i = 0; i < (s%omp_get_max_threads());i++){
    x = get_random();
    y = get_random();
    double temp = pow(x,2)+pow(y,2);
    if(temp <= 1.0){
      hit++;
    }
  }
  return (double) (hit*4)/s;
}