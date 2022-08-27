#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include "../utils/defs.h"
#include "ray.h"

class camera
{
public:
    camera()
    {
        origin = point3(0, 0, 0);
        horizontal = vec3(constants::viewport_width, 0, 0);
        vertical = vec3(0, constants::viewport_height, 0);
        lower_left = origin - horizontal / 2.0 - vertical / 2.0 - vec3(0, 0, constants::focal_length);
    }

    ray get_ray(double u, double v) const
    {
        return ray(origin, lower_left + u * horizontal + v * vertical - origin);
    }

private:
    point3 origin;
    point3 lower_left;
    vec3 horizontal;
    vec3 vertical;
};

#endif // CAMERA_H_INCLUDED
