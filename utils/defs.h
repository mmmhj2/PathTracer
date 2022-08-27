#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#include "vec3d.h"

namespace constants
{

constexpr double aspect_ratio = 16.0 / 9.0;
constexpr int image_width = 400;
constexpr int image_height = image_width / aspect_ratio;

constexpr double viewport_height = 2.0;
constexpr double viewport_width = viewport_height * aspect_ratio;
constexpr double focal_length = 1.0;

const vec3 origin = point3 (0, 0, 0);
const vec3 horizontal = vec3 (viewport_width, 0, 0);
const vec3 vertical = vec3 (0, viewport_height, 0);
const vec3 lower_left = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

constexpr double dinf = std::numeric_limits<double>::infinity();

};

#endif // DEFS_H_INCLUDED
