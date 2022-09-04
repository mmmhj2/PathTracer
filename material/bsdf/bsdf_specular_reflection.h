#ifndef BSDF_SPECULAR_REFLECTION_H_INCLUDED
#define BSDF_SPECULAR_REFLECTION_H_INCLUDED

#include "bsdf_base.h"

class BSDF_specular_reflection : public BSDF_delta_base
{
    std::shared_ptr <uv_texture> tex;
public:
    BSDF_specular_reflection(const hit_record & _rec, std::shared_ptr <uv_texture> _tex)
    : BSDF_delta_base(_rec), tex(_tex) {}

    virtual color eval(const ray &, const ray &) const
    {
        return tex->get_color(rec.u, rec.v, rec.p);
    }

    virtual void sample(const ray & o, ray & sample) const
    {
        vec3 reflected = tools::reflect(o.direction().unit(), rec.normal);
        sample = ray(rec.p, reflected);
    }
};

#endif // BSDF_SPECULAR_REFLECTION_H_INCLUDED
