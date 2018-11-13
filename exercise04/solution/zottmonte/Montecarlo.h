#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <gmp.h>
#include <omp.h>
#include <random>

#ifndef MONTECARLO_H
#define MONTECARLO_H

double get_random();
void montecarloSeq(long long s,mpf_t result);
void montecarloPar(long long s,mpf_t result);
double getDoubleSeq(long long s);
double getDoublePar(long long s);

#endif //MONTECARLO_H
