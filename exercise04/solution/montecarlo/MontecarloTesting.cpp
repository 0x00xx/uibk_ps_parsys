// g++ montecarlo_par2.cpp -o montecarlo_par -O3 -std=c++11 -Wall -lgmp -fopenmp

#include "Montecarlo.h"

using namespace std;

double getDoubleSeq(long long s);
double getDoublePar(long long s);

/*double get_random(){
  // static std::random_device rd;
  // static std::mt19937 e(rd());
  static std::mt19937 e(123456789);
  static std::uniform_real_distribution<> dis(0, 1); // range 0 - 1
  return dis(e);
}*/

double get_random(){
  return ((double) rand() / (RAND_MAX));
}

void montecarloSeq(long long s,mpf_t result)
{
  long long hit = 0;
  double x;
  double y;
  // Check if it is in circle
  for (long long i=0; i<s; i++) {
    srand (12345+i);
    x = get_random();
    y = get_random();
    double temp = pow(x,2)+pow(y,2);
    if(temp <= 1.0){
      hit++;
    }
  }
  // Big int division
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

void montecarloPar(long long s,mpf_t result){
  long long hit = 0;
  double x;
  double y;
  double temp;
  long long i;
  // Check if it is in circle
  #pragma omp parallel private(temp, x, y, i) reduction (+:hit)
  {
    x = 0.0;
    y = 0.0;
    temp = 0.0;
    #pragma omp parallel for
    for (i=0; i<s/omp_get_max_threads(); i++) {
      srand (12345+i);
      x = get_random();
      y = get_random();
      temp = pow(x,2)+pow(y,2);
      if(temp <= 1.0){
      //#pragma omp atomic
        hit++;
      }
    }
  }
  for(i = 0; i < (s%omp_get_max_threads());i++){
    srand (12345+i);
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

double getDoubleSeq(long long s){
  mpf_t result;
  montecarloSeq(s,result);
  double pi = mpf_get_d(result);
  return pi;
}

double getDoublePar(long long s){
  mpf_t result;
  // s = s / omp_get_max_threads();
  montecarloPar(s,result);
  double pi = mpf_get_d(result);
  return pi;
}