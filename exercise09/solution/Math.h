//
// Created by bennett on 11/22/17.
//

#ifndef PARALLEL_MATH_H
#define PARALLEL_MATH_H

#include <vector>
/// @file Math.h math helpers

/**
 * Check whether delta between iterations is below epsilon
 *
 * (termination criterion)
 *
 * @param before values before iteration
 * @param after  values after iteration
 * @return       whether the jacobi process can be terminated
 */
double deltaPar(const std::vector<double> &before, const std::vector<double> &after,
                       int upperghosts, int lowerghosts);

/**
 * Check whether delta between iterations is below epsilon
 *
 * (termination criterion)
 *
 * @param before values before iteration
 * @param after  values after iteration
 * @return       whether the jacobi process can be terminated
 */
bool deltaBelowEpsilon(double epsilon, const std::vector<double> &before, const std::vector<double> &after);

/**
 * Calculate new value from old values
 *
 * @param x      old value
 * @param xLeft  old value left of point
 * @param xRight old value right of point
 * @return new value
 */
inline double calc1D(const double x, const double xLeft, const double xRight)
{
    return (xLeft+xRight+x)/3.0;
}

/**
 *  Calculate new value from old values
 *
 * @param x       old value
 * @param xLeft   old value left of point
 * @param xRight  old value right of point
 * @param xUp     old value above point
 * @param xDown   old value below point
 * @return new value
 */
inline double calc2D(const double x, const double xLeft, const double xRight, const double xUp, const double xDown)
{
    return (x+xLeft+xRight+xUp+xDown)/5.0;
}

/**
 * Calculate new value from old values
 *
 * @param x       old value
 * @param xLeft   old value left of point
 * @param xRight  old value right of point
 * @param xUp     old value above point
 * @param xDown   old value below point
 * @param xFront  old value in front of point
 * @param xBehind old value behind point
 * @return new value
 */
inline double calc3D(const double x, const double xLeft, const double xRight, const double xUp, const double xDown, const double xFront,
              const double xBehind)
{
    return (x+xLeft+xRight+xUp+xDown+xFront+xBehind)/7.0;
}
#endif //PARALLEL_MATH_H
