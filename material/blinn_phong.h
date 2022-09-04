#ifndef BLINN_PHONG_H_INCLUDED
#define BLINN_PHONG_H_INCLUDED

#include "material_base.h"
#include "ray/ray.h"
#include "objects/object_base.h"

#include <cassert>

class blinn_phong : public non_emissive_material
{
private:
    std::shared_ptr <uv_texture> k_L, k_G;
    int s;

public:
    blinn_phong(std::shared_ptr <uv_texture> _k_L, std::shared_ptr <uv_texture> _k_G, int _s)
    : k_L(_k_L), k_G(_k_G), s(_s)
    {
    }

    virtual bool evaluateScatter(const ray & incident,
                                 const hit_record & rec,
                                 std::shared_ptr <BSDF_base> & bsdf) const override;
};

bool blinn_phong::evaluateScatter(const ray& incident, const hit_record& rec, std::shared_ptr <BSDF_base> & bsdf) const
{
    if(tools::random_double() <= 0.5)
        bsdf = std::make_shared<BSDF_lambertian>(rec, k_L);
    else
        bsdf = std::make_shared<BSDF_BP_specular>(rec, s, k_G);
    return true;
}


#endif // BLINN_PHONG_H_INCLUDED
