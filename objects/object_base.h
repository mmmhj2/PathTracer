#ifndef OBJECT_BASE_H_INCLUDED
#define OBJECT_BASE_H_INCLUDED

#include "../ray/ray.h"

struct hit_record
{
    point3 p;
    vec3 normal;
    double t;
    bool is_front;

    void set_normal(const ray & incident, const vec3 & normal_out)
    {
        is_front = (incident.direction() * normal_out < 0);
        normal = (is_front ? normal_out : -normal_out);
    }
};

class hittable_object
{
public:
    virtual bool hit (const ray & r, double t_min, double t_max, hit_record & rec) const = 0;
};

#endif // OBJECT_BASE_H_INCLUDED
