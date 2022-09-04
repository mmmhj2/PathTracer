#ifndef BSDF_SPECULAR_REFRACTION_H_INCLUDED
#define BSDF_SPECULAR_REFRACTION_H_INCLUDED

#include "bsdf_base.h"

class BSDF_specular_refraction : public BSDF_delta_base
{
    double eta;
    std::shared_ptr <uv_texture> tex;

    static double schlick_approximation(double cosine, double ior)
    {
        double r0 = (1 - ior) / (1 + ior);
        r0 *= r0;
        return r0 + (1-r0) * std::pow((1-cosine), 5);
    }

public:
    BSDF_specular_refraction(const hit_record & _rec, double _eta, std::shared_ptr <uv_texture> _tex)
    : BSDF_delta_base(_rec), eta(_eta), tex(_tex) {}

    virtual color eval(const ray &, const ray &) const
    {
        return tex->get_color(rec.u, rec.v, rec.p);
    }
    virtual void sample(const ray & o, ray & sample) const
    {
        double actual_ior = rec.is_front ? (1.0 / eta) : eta;

        vec3 unit_direction = o.direction().unit();
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

        sample = ray(rec.p, direction);
    }
};

#endif // BSDF_SPECULAR_REFRACTION_H_INCLUDED
