#ifndef BSDF_LAMBERTIAN_H_INCLUDED
#define BSDF_LAMBERTIAN_H_INCLUDED

#include "utils/orthonormal_basis.h"
#include "bsdf_base.h"
#include "../texture/texture_base.h"

class BSDF_lambertian : public BSDF_base
{
    std::shared_ptr <uv_texture> tex;
public:
    BSDF_lambertian(const hit_record & _rec, std::shared_ptr <uv_texture> _tex)
    : BSDF_base(_rec), tex(_tex) {};

    virtual color eval([[maybe_unused]] const ray & o, [[maybe_unused]] const ray & i) const
    {
        return tex->get_color(rec.u, rec.v, rec.p);
    }

    virtual double pdf([[maybe_unused]] const ray & o, const ray & i) const override
    {
        double cosine = (i.direction().unit() * rec.normal.unit());
        return (cosine < 0 ? 0 : cosine / constants::pi);
    }

    virtual void sample(const ray & o, ray & spl) const override
    {
        // Cosine weighted sample
        onb uvw(rec.normal);
        double r1, r2, z, phi;
        r1 = tools::random_double();
        r2 = tools::random_double();
        z = std::sqrt(1 - r2);
        phi = 2 * constants::pi * r1;

        vec3 dir = uvw.local(vec3(std::cos(phi) * std::sqrt(r2), std::sin(phi) * std::sqrt(r2), z));
        spl = ray(rec.p, dir);
    }
};

#endif // BSDF_LAMBERTIAN_H_INCLUDED
