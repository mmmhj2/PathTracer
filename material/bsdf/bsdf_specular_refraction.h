#ifndef BSDF_SPECULAR_REFRACTION_H_INCLUDED
#define BSDF_SPECULAR_REFRACTION_H_INCLUDED

#include "bsdf_base.h"

class BSDF_specular_refraction : public BSDF_delta_base
{
    double eta;
    std::shared_ptr <uv_texture> tex;

    bool is_reflect(const vec3 & unit_direction) const
    {
        double actual_ior = rec.is_front ? (1.0 / eta) : eta;

        double cos_theta = std::min((-unit_direction) * rec.normal, 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

        return actual_ior * sin_theta >= 1.0;
    }

public:
    BSDF_specular_refraction(const hit_record & _rec, double _eta, std::shared_ptr <uv_texture> _tex)
    : BSDF_delta_base(_rec), eta(_eta), tex(_tex) {}

    virtual
    color eval(const ray &o, const ray &) const
    {
        return tex->get_color(rec.u, rec.v, rec.p);
    }
    virtual void sample(const ray & o, ray & sample) const
    {
        double actual_ior = rec.is_front ? (1.0 / eta) : eta;

        vec3 unit_direction = o.direction().unit();
        vec3 direction;

        if(is_reflect(unit_direction))
            direction = tools::reflect(unit_direction, rec.normal);
        else
            direction = tools::refract(unit_direction, rec.normal, actual_ior);

        sample = ray(rec.p, direction);
    }
};

#endif // BSDF_SPECULAR_REFRACTION_H_INCLUDED
