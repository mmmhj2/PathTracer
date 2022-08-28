#ifndef BLINN_PHONG_H_INCLUDED
#define BLINN_PHONG_H_INCLUDED

#include "material_base.h"

class blinn_phong_naive : public material
{
private:
    color k_L, k_G;
    double s;

public:
    blinn_phong_naive(color _k_L, color _k_G, double _s)
    : k_L(std::move(_k_L)), k_G(std::move(_k_G)), s(_s)
    {
    }

    virtual bool evaluateScatter(const ray & incident,
                                 const hit_record & rec,
                                 color & attenuation,
                                 ray & scattered) const override;
};

bool blinn_phong_naive::evaluateScatter(const ray& incident, const hit_record& rec, color& attenuation, ray& scattered) const
{
    // PDF is the same as Lambertian
    // Doesn't support importance sampling, and therefore specular rendered is marginal
    auto scatter_dir = rec.normal + vec3::random_unit();
    if(tools::is_near_zero(scatter_dir))
        scatter_dir = rec.normal;

    scattered = ray(rec.p, scatter_dir);

    // Compute z and z^s
    auto h = (incident.direction() + scattered.direction()) / (incident.direction().norm() + scattered.direction().norm());
    double z = std::max(0.0, h * rec.normal.unit());
    double zs = std::pow(z, s);

    attenuation = (k_L + k_G * ((8.0 + s) / 8.0) * zs);
    return true;
}


#endif // BLINN_PHONG_H_INCLUDED
