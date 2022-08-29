#include "aabb.h"
#include "../object_base.h"

const point3 & aabb::min() const {return minimum;}
const point3 & aabb::max() const {return maximum;}

bool aabb::hit(const ray & r, double t_min, double t_max) const
{
    for(size_t i = 0; i < 3; i++)
    {
        double inv = 1.0 / r.direction()[i];
        double t0 = (minimum[i] - r.origin()[i]) * inv;
        double t1 = (maximum[i] - r.origin()[i]) * inv;

        if(inv < 0.0)
            std::swap(t0, t1);

        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;
        if(t_max <= t_min)
            return false;
    }
    return true;
}

aabb aabb::surrounding_box(const aabb & box0, const aabb & box1)
{
    point3 s(std::min(box0.min()[0], box1.min()[0]),
             std::min(box0.min()[1], box1.min()[1]),
             std::min(box0.min()[2], box1.min()[2]));
    point3 b(std::max(box0.max()[0], box1.max()[0]),
             std::max(box0.max()[1], box1.max()[1]),
             std::max(box0.max()[2], box1.max()[2]));
    return aabb(s, b);
}

bool aabb::compare_box_axis(const hittable_object * a,const hittable_object * b, int axis)
{
    aabb box_a, box_b;
    if(!a->get_aabb(box_a) || !b->get_aabb(box_b))
        return false;

    return box_a.min()[axis] < box_b.min()[axis];
}
