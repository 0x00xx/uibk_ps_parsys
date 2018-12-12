
#ifndef PARALLEL_MATH_H
#define PARALLEL_MATH_H

#include <vector>
/// @file Math.h math helpers


//Check whether delta between iterations is below epsilon
double deltaPar(const std::vector<double> &before, const std::vector<double> &after,
                       int upperghosts, int lowerghosts);

//Check whether delta between iterations is below epsilon
bool deltaBelowEpsilon(double epsilon, const std::vector<double> &before, const std::vector<double> &after);


inline double calc1D(const double x, const double xLeft, const double xRight){
    return (xLeft+xRight+x)/3.0;
}


inline double calc2D(const double x, const double xLeft, const double xRight, const double xUp, const double xDown){
    return (x+xLeft+xRight+xUp+xDown)/5.0;
}


inline double calc3D(const double x, const double xLeft, const double xRight, const double xUp, const double xDown, const double xFront,
              const double xBehind){
    return (x+xLeft+xRight+xUp+xDown+xFront+xBehind)/7.0;
}
#endif //PARALLEL_MATH_H
