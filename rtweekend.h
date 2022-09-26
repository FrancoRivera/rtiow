#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib> // to get random (old)
#include <random>  // to get random (new cpp)

// usings
//
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932384626433;

// Utility Functions

inline double degrees_to_radians(double degrees){
    return degrees * pi / 180.0;
}

// old way, rand() is not really random btw unless you seed it, same as mt19937
inline double random_double(){
    // returns a random real in [0,1];
    return rand() / (RAND_MAX + 1.0);
}
// new way
inline double random_double_new(){
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator; // marsene twister 19937 32 bit words
    return distribution(generator);
}

inline double random_double(double min, double max){
    // returns a random real in [min,max];
    return min + (max-min) * random_double();
}

inline double clamp(double x, double min, double max){
    if (x < min) return min;
    if (x > max) return max;
    return x;
}


// Common Headers
#include "ray.h"
#include "vec3.h"

#endif // RTWEEKEND_H
