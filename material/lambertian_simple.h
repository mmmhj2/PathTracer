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
                                 std::shared_ptr <BSDF_base> & bsdf) const override
    {
        bsdf = std::make_shared <BSDF_lambertian>(rec, k_L);
        return true;
    }
private:
    std::shared_ptr <uv_texture> k_L;
};


#endif // LAMBERTIAN_SIMPLE_H_INCLUDED
