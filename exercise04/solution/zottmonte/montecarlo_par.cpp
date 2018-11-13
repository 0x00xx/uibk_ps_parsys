// g++ montecarlo_par2.cpp -o montecarlo_par -O3 -std=c++11 -Wall -lgmp -fopenmp

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <gmp.h>
#include <omp.h>
#include <random>

double get_random(){
  // static std::random_device rd;
  // static std::mt19937 e(rd());
  static std::mt19937 e(123456789);
  static std::uniform_real_distribution<> dis(0, 1); // range 0 - 1
  return dis(e);
}

void montecarlo(long long s,mpf_t result){
  long long hit = 0;
  double x;
  double y;
  // Check if it is in circle
  #pragma omp for
  for (long long i=0; i<s; i++) {
    x = get_random();
    y = get_random();
    double temp = pow(x,2)+pow(y,2);
    if(temp <= 1.0){
      hit++;
    }
  }
  // Big int divison
  mpf_t hit_big;
  mpf_t s_big;
  mpf_init(hit_big);
  mpf_init(s_big);
  mpf_init2(result,256);
  mpf_set_ui(hit_big,hit);
  mpf_mul_ui(hit_big,hit_big,4);
  mpf_set_ui(s_big,s);
  mpf_div(result,hit_big,s_big);
}

double getDouble(long long s){
  mpf_t result;
  s = s / omp_get_max_threads();
  montecarlo(s,result);
  double pi = mpf_get_d(result);
  return pi;
}