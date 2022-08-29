#ifndef SPHERE_H_INCLUDED
#define SPHERE_H_INCLUDED

#include "object_base.h"

class sphere : public hittable_object
{
public:
    point3 center;
    double radius;
    std::shared_ptr <material> material_ptr;

public:
    sphere() {};
    sphere(point3 c, double r, std::shared_ptr <material> mat)
        : center(std::move(c)), radius(r), material_ptr(mat)
    {};

    virtual bool hit(const ray & r, double t_min, double t_max, hit_record & rec) const override;
    virtual bool get_aabb(aabb & output) const;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
    vec3 oc = r.origin() - center;
    double a = r.direction().norm_squared();
    double b_half = oc * r.direction();
    double c = oc.norm_squared() - radius * radius;

    double delta = b_half * b_half - a * c;
    if(delta < 0)
        return false;
    double sq = std::sqrt(delta);

    double root = (-b_half - sq) / a;
    if(root < t_min || root > t_max)
    {
        root = (-b_half + sq) / a;
        if(root < t_min || root > t_max)
            return false;
    }

    rec.t = root;
    rec.p = r.at(root);
    //rec.normal = (rec.p - center) / radius;
    rec.mat = material_ptr;
    rec.set_normal(r, (rec.p - center) / radius);
    //std::cout << "ray : " << r.direction() << " intersect at : " << rec.p << " normal : " << rec.normal << std::endl ;
    return true;
}

bool sphere::get_aabb(aabb& output) const
{
    output = aabb(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
    return true;
}


#endif // SPHERE_H_INCLUDED
