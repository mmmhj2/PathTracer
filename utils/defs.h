#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#include "vec3d.h"
#include <random>
#include <thread>
#include <cmath>

namespace constants
{

constexpr double pi = std::atan(1)*4;

constexpr double aspect_ratio = 16.0 / 9.0;
constexpr int image_width = 800;
constexpr int image_height = image_width / aspect_ratio;

//constexpr double viewport_height = 2.0;
//constexpr double viewport_width = viewport_height * aspect_ratio;
//constexpr double focal_length = 1.0;

constexpr int sample_per_pixel = 100;

const int blocks = std::thread::hardware_concurrency() - 1;
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
constexpr double EPS = 1e-6;

template <typename T>
inline T clamp (T x, T min, T max)
{
    x = (x > max ? max : x);
    return (x < min ? min : x);
}

double random_double();
double random_double(double min, double max);
int random_int(int min, int max);
vec3 reflect(const vec3 & v, const vec3 & n);
vec3 refract(const vec3 &uv, const vec3 & n, double etai_etat);
bool is_near_zero(const vec3 & v);

};

#endif // DEFS_H_INCLUDED
