#ifndef AABB_H_INCLUDED
#define AABB_H_INCLUDED

#include "utils/vec3d.h"

class ray;
class hittable_object;

class aabb
{
private:
    point3 minimum;
    point3 maximum;

public:

    aabb() = default;
    aabb(const point3 & min, const point3 & max)
    : minimum(min), maximum(max)
    {
    }

    const point3 & min() const;
    const point3 & max() const;

    bool hit(const ray & r, double t_min, double t_max) const;
    static aabb surrounding_box(const aabb & box0, const aabb & box1);
    static bool compare_box_axis(const hittable_object * a,const hittable_object * b, int axis);
};


#endif // AABB_H_INCLUDED
