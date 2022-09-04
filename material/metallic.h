#ifndef METALLIC_H_INCLUDED
#define METALLIC_H_INCLUDED

#include "material_base.h"

class metallic : public non_emissive_material
{
private:
    std::shared_ptr <uv_texture> albedo;

public:
    metallic(std::shared_ptr <uv_texture> alb) : albedo(alb)
    {
    }

    virtual color getAlbedo(const hit_record & rec) const override
    {
        return albedo->get_color(rec.u, rec.v, rec.p);
    }

    virtual bool evaluateScatter(const ray & incident, const hit_record & rec, std::shared_ptr <BSDF_base> & bsdf) const override;
};

bool metallic::evaluateScatter(const ray& incident, const hit_record& rec, std::shared_ptr <BSDF_base> & bsdf) const
{
    bsdf = std::make_shared<BSDF_specular_reflection>(rec, albedo);
    return true;
}


#endif // METALLIC_H_INCLUDED
