#ifndef RAY_TRACING_IN_ONE_WEEKEND_H
#define RAY_TRACING_IN_ONE_WEEKEND_H

#include <cmath>
#include <limits>
#include <memory>
#include <random>

// using
//----------------------------------------------------------

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// constants
//----------------------------------------------------------

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// utility functions
//----------------------------------------------------------

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.;
}

/**
 * return a random real in [0,1)
 */
inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0., 1.);
    static std::mt19937 generator;
    return distribution(generator);
}

/**
 * return a random real in [min, max) 
 */
inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

// common headers
//----------------------------------------------------------

#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif
