#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include "../utils/defs.h"
#include "ray.h"

class camera
{
public:
    camera(point3 from,
           point3 at,
           vec3 up,
           double vfov = constants::pi / 2,
           double aspect_radio = constants::aspect_ratio
           )
    {
        auto h = std::tan(vfov / 2);
        double viewport_height = 2.0 * h;
        double viewport_width = aspect_radio * viewport_height;

        vec3 w, u, v;
        w = (from - at).unit();
        u = (up ^ w).unit();
        v = w ^ u;

        origin = from;
        horizontal = viewport_width * u;
        vertical = viewport_height * v;
        lower_left = origin - horizontal / 2.0 - vertical / 2.0 - w;
    }

    ray get_ray(double s, double t) const
    {
        return ray(origin, lower_left + s * horizontal + t * vertical - origin);
    }

private:
    point3 origin;
    point3 lower_left;
    vec3 horizontal;
    vec3 vertical;
};

#endif // CAMERA_H_INCLUDED
