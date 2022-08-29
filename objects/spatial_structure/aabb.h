#ifndef AABB_H_INCLUDED
#define AABB_H_INCLUDED

#include "utils/vec3d.h"

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

    const point3 & min() const {return minimum;}
    const point3 & max() const {return maximum;}

    bool hit(const ray & r, double t_min, double t_max) const
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

    static aabb surrounding_box(const aabb & box0, const aabb & box1)
    {
        point3 s(std::min(box0.min()[0], box1.min()[0]),
                 std::min(box0.min()[1], box1.min()[1]),
                 std::min(box0.min()[2], box1.min()[2]));
        point3 b(std::max(box0.max()[0], box1.max()[0]),
                 std::max(box0.max()[1], box1.max()[1]),
                 std::max(box0.max()[2], box1.max()[2]));
        return aabb(s, b);
    }
};


#endif // AABB_H_INCLUDED
