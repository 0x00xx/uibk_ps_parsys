#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include <random>

#ifndef MONTECARLO_H
#define MONTECARLO_H

double get_random();
double montecarloSeq(long s);
double montecarloPar(long s);

#endif //MONTECARLO_H
