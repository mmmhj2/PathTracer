#ifndef DIELECTRIC_H_INCLUDED
#define DIELECTRIC_H_INCLUDED

#include "material_base.h"

class dielectric : public non_emissive_material
{
private:
    double ior;
    std::shared_ptr <uv_texture> albedo;

public:
    dielectric(double index_of_refraction, std::shared_ptr <uv_texture> alb)
    : ior(index_of_refraction), albedo(alb) {};
    virtual bool evaluateScatter(const ray & incident,
                                 const hit_record & rec,
                                 std::shared_ptr <BSDF_base> & bsdf) const override;
};

bool dielectric::evaluateScatter(const ray& incident, const hit_record& rec, std::shared_ptr <BSDF_base> & bsdf) const
{
    bsdf = std::make_shared<BSDF_specular_refraction>(rec, ior, albedo);
    return true;
}


#endif // DIELECTRIC_H_INCLUDED
