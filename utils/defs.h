#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#include "vec3d.h"
#include <random>

namespace constants
{

constexpr double aspect_ratio = 16.0 / 9.0;
constexpr int image_width = 400;
constexpr int image_height = image_width / aspect_ratio;

constexpr double viewport_height = 2.0;
constexpr double viewport_width = viewport_height * aspect_ratio;
constexpr double focal_length = 1.0;

constexpr int sample_per_pixel = 100;
/*
const vec3 origin = point3 (0, 0, 0);
const vec3 horizontal = vec3 (viewport_width, 0, 0);
const vec3 vertical = vec3 (0, viewport_height, 0);
const vec3 lower_left = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
*/
constexpr double dinf = std::numeric_limits<double>::infinity();

};

namespace tools
{
template <typename T>
inline T clamp (T x, T min, T max)
{
    x = (x > max ? max : x);
    return (x < min ? min : x);
}

inline double random_double()
{
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    static std::mt19937 generator;
    return dist(generator);
}

inline double random_double(double min, double max)
{
    static std::mt19937 generator;
    return std::uniform_real_distribution<double>(min, max)(generator);
}

};

#endif // DEFS_H_INCLUDED
