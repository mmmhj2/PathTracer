#ifndef DIELECTRIC_H_INCLUDED
#define DIELECTRIC_H_INCLUDED

#include "material_base.h"

class dielectric : public non_emissive_material
{
private:
    double ior;
    std::shared_ptr <uv_texture> albedo;

    static double schlick_approximation(double cosine, double ior)
    {
        double r0 = (1 - ior) / (1 + ior);
        r0 *= r0;
        return r0 + (1-r0) * std::pow((1-cosine), 5);
    }


public:
    dielectric(double index_of_refraction, std::shared_ptr <uv_texture> alb)
    : ior(index_of_refraction), albedo(alb) {};

    virtual color getAlbedo(const hit_record & rec) const override
    {
        return albedo->get_color(rec.u, rec.v, rec.p);
    }

    virtual bool evaluateScatter(const ray & out,
                                 const hit_record & rec,
                                 std::shared_ptr <BSDF_base> & bsdf) const override;
};

bool dielectric::evaluateScatter(const ray& out, const hit_record& rec, std::shared_ptr <BSDF_base> & bsdf) const
{
    //bsdf = std::make_shared<BSDF_specular_refraction>(rec, ior, albedo);

    double actual_ior = rec.is_front ? (1 / ior) : ior;
    double cosine_out = std::min(-out.direction().unit() * rec.normal.unit(), 1.0);
    double R_f = schlick_approximation(cosine_out, actual_ior);

    if(R_f > tools::random_double())
        bsdf = std::make_shared<BSDF_specular_reflection>(rec, albedo);
    else
        bsdf = std::make_shared<BSDF_specular_refraction>(rec, ior, albedo);

    return true;
}


#endif // DIELECTRIC_H_INCLUDED
