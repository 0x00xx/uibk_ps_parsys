
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <iostream>
#include "Math.h"

bool deltaBelowEpsilon(const double epsilon, const std::vector<double> &before, const std::vector<double> &after)
{
    assert(epsilon > 0);
    long double delta = 0;
    for (unsigned long i = 0; i < before.size(); ++i) {
        delta += fabs(before.at(i) - after.at(i));
    }
    //std::cout << delta << std::endl;
    return epsilon > delta;
}

double deltaPar(const std::vector<double> &before, const std::vector<double> &after, int upperghosts, int lowerghosts)
{
    //std::cout << "Delta 3D " << after.size()-lowerghosts-upperghosts << std::endl;
    double delta = 0;
    for (unsigned long i = 0 + lowerghosts; i < before.size() - upperghosts; ++i) {
        delta += fabs(before.at(i) - after.at(i));
    }
    //std::cout << "DELTA " << delta << std::endl;
    return delta;
}
