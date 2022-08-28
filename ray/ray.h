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
constexpr double EPS = 1e-6;

vec3 reflect(const vec3 & v, const vec3 & n)
{
    return v - 2 * (v * n) * n;
}

vec3 refract(const vec3 &uv, const vec3 & n, double etai_etat)
{
    double cos_theta = std::min((-uv) * n, 1.0);
    vec3 out_perp = etai_etat * (uv + cos_theta * n);
    vec3 out_para = -std::sqrt(std::fabs(1.0 - out_perp.norm_squared())) * n;

    return out_perp + out_para;
}

bool is_near_zero(const vec3 & v)
{
    return ((fabs(v[0]) < EPS) && (fabs(v[1]) < EPS) && (fabs(v[2]) < EPS));
}
}

#endif // RAY_H_INCLUDED
