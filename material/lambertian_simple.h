#ifndef LAMBERTIAN_SIMPLE_H_INCLUDED
#define LAMBERTIAN_SIMPLE_H_INCLUDED

#include "material_base.h"
#include "texture/texture_base.h"
#include "objects/object_base.h"

class lambertian : public non_emissive_material
{
public:
    lambertian(std::shared_ptr <uv_texture> clr) : k_L(clr)
    {
    }

    virtual bool evaluateScatter(const ray & incident,
                                 const hit_record & rec,
                                 color & attenuation,
                                 ray & scattered) const override
    {
        // PDF of random_unit() is cos(theta) / PI
        // Where theta is the angle between normal and scattered ray
        auto scatter_dir = rec.normal + vec3::random_unit();

        if(tools::is_near_zero(scatter_dir))
            scatter_dir = rec.normal;

        scattered = ray(rec.p, scatter_dir);
        attenuation = k_L->get_color(rec.u, rec.v, rec.p);
        return true;
    }
private:
    std::shared_ptr <uv_texture> k_L;
};


#endif // LAMBERTIAN_SIMPLE_H_INCLUDED
