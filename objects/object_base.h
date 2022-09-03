#ifndef OBJECT_BASE_H_INCLUDED
#define OBJECT_BASE_H_INCLUDED

#include "spatial_structure/aabb.h"
#include "ray/ray.h"
#include "utils/defs.h"
#include <memory>

class material;

struct hit_record
{
    point3 p;
    // Normal vector of p, norm should be 1
    vec3 normal;
    double t;
    double u, v;
    std::weak_ptr <material> mat;
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
    virtual bool get_aabb(aabb & output) const = 0;
    virtual point3 get_centroid() const = 0;

    virtual double pdf_value(const point3 & o, const vec3 & v) const
    {
        ray r(o, v);
        hit_record rec;
        if(!this->hit(r, 0.001, constants::dinf, rec))
            return 0;
        return this->pdf_value(r, rec);
    }

    virtual double pdf_value(const ray & r, const hit_record & h) const = 0;
    // Generate a point in the object in global frame
    virtual vec3 sample() const = 0;
};

#endif // OBJECT_BASE_H_INCLUDED
