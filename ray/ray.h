#ifndef RAY_H_INCLUDED
#define RAY_H_INCLUDED

#include "../utils/vec3d.h"

class ray
{
public:
    point3 orig;
    vec3 dir;

public:
    ray() {};
    ray(const point3 & origin, const vec3 & direction)
        : orig(origin), dir(direction)
    {};

    point3 origin() const
    {
        return orig;
    }

    vec3 direction() const
    {
        return dir;
    }

    point3 at(double t) const
    {
        return orig + dir * t;
    }
};

namespace tools
{
vec3 reflect(const vec3 & v, const vec3 & n)
{
    return v - 2 * (v * n) * n;
}
}

#endif // RAY_H_INCLUDED
