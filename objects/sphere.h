#ifndef SPHERE_H_INCLUDED
#define SPHERE_H_INCLUDED

#include "object_base.h"

class sphere : public hittable_object
{
public:
    point3 center;
    double radius;
    std::shared_ptr <material> material_ptr;

    static std::pair <double, double> get_sphere_uv(const point3 & point_local)
    {
        double theta, phi;
        theta = std::acos(-point_local[1]);
        phi = std::atan2(-point_local[2], point_local[0]) + constants::pi;
        return std::make_pair(phi / (2 * constants::pi), theta / constants::pi);
    }

    static vec3 random_to_sphere(double radius, double distance_sq)
    {
        double r1, r2, z, phi, x, y;
        r1 = tools::random_double();
        r2 = tools::random_double();
        z = 1 + r2*(std::sqrt(1-radius*radius/distance_sq) - 1);
        phi = 2 * constants::pi * r1;
        x = std::cos(phi) * std::sqrt(1 - z * z);
        y = std::sin(phi) * std::sqrt(1 - z * z);
        return vec3(x, y, z);
    }

public:
    sphere() {};
    sphere(point3 c, double r, std::shared_ptr <material> mat)
        : center(c), radius(r), material_ptr(mat)
    {};

    virtual bool hit(const ray & r, double t_min, double t_max, hit_record & rec) const override;
    virtual bool get_aabb(aabb & output) const;

    virtual point3 get_centroid() const override;

    virtual double pdf_value(const ray & r, const hit_record & h) const override
    {
        double cos_theta_max = std::sqrt(1 - radius * radius / (center - r.origin()).norm_squared());
        double solid_angle = 2 * constants::pi * (1 - cos_theta_max);
        return 1 / solid_angle;
    }

    virtual vec3 sample(const point3 & from) const override
    {
        vec3 dir = center - from;
        double distance_sq = dir.norm_squared();
        onb uvw(dir);
        return uvw.local(random_to_sphere(radius, distance_sq));
    }
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
    rec.mat = material_ptr;
    vec3 normal = (rec.p - center) / radius;
    rec.set_normal(r, normal);
    std::tie(rec.u, rec.v) = get_sphere_uv(normal);
    //std::cout << "ray : " << r.direction() << " intersect at : " << rec.p << " normal : " << rec.normal << std::endl ;
    //std::cout << "UV : " << rec.u << " " << rec.v << std::endl ;
    return true;
}

bool sphere::get_aabb(aabb& output) const
{
    double rad = std::abs(radius);
    output = aabb(center - vec3(rad, rad, rad), center + vec3(rad, rad, rad));
    return true;
}

point3 sphere::get_centroid() const
{
    return this->center;
}



#endif // SPHERE_H_INCLUDED
