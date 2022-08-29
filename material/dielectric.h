#ifndef DIELECTRIC_H_INCLUDED
#define DIELECTRIC_H_INCLUDED

#include "material_base.h"

class dielectric : public material
{
private:
    double ior;
    color albedo;

    static double schlick_approximation(double cosine, double ior)
    {
        double r0 = (1 - ior) / (1 + ior);
        r0 *= r0;
        return r0 + (1-r0) * std::pow((1-cosine), 5);
    }

public:
    dielectric(double index_of_refraction, color alb = color(1.0, 1.0, 1.0))
    : ior(index_of_refraction), albedo(alb) {};
    virtual bool evaluateScatter(const ray & incident,
                                 const hit_record & rec,
                                 color & attenuation,
                                 ray & scattered) const override;
};

bool dielectric::evaluateScatter(const ray& incident, const hit_record& rec, color& attenuation, ray& scattered) const
{
    attenuation = albedo;
    double actual_ior = rec.is_front ? (1.0 / ior) : ior;

    vec3 unit_direction = incident.direction().unit();
    vec3 direction;

    double cos_theta = std::min((-unit_direction) * rec.normal, 1.0);
    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    bool is_reflect =
        schlick_approximation(cos_theta, actual_ior) > tools::random_double()
        || actual_ior * sin_theta > 1.0;
    if(is_reflect)
        direction = tools::reflect(unit_direction, rec.normal);
    else
        direction = tools::refract(unit_direction, rec.normal, actual_ior);

    scattered = ray(rec.p, direction);
    return true;
}


#endif // DIELECTRIC_H_INCLUDED
