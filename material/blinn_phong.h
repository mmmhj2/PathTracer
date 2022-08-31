#ifndef BLINN_PHONG_H_INCLUDED
#define BLINN_PHONG_H_INCLUDED

#include "material_base.h"
#include "ray/ray.h"
#include "objects/object_base.h"

#include <cassert>

class blinn_phong_naive : public non_emissive_material
{
private:
    color k_L, k_G;
    double s;

public:
    blinn_phong_naive(color _k_L, color _k_G, double _s)
    : k_L(_k_L), k_G(_k_G), s(_s)
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
    //assert(rec.normal.norm() <= 1);
    auto scatter_dir = rec.normal + vec3::random_unit();
    if(tools::is_near_zero(scatter_dir))
        scatter_dir = rec.normal;

    scattered = ray(rec.p, scatter_dir);

    // Compute z and z^s
    auto h = (-incident.direction() + scattered.direction()) / (incident.direction().norm() + scattered.direction().norm());
    double z = std::max(0.0, h * rec.normal.unit());
    double zs = std::pow(z, s);

#ifdef BLINN_PHONG_DEBUG
    std::cerr << "Incident : " << incident.origin() << " to " << incident.direction() << std::endl ;
    std::cerr << "Hit at : " << rec.p << " normal : " << rec.normal << std::endl;
    std::cerr << "Scatter : " << scatter_dir << std::endl ;
    std::cerr << "Half vector : " << h << std::endl ;
    std::cerr << "z : " << z << std::endl ;
#endif

    attenuation = (k_L + k_G * ((8.0 + s) / 8.0) * zs);
    return true;
}


#endif // BLINN_PHONG_H_INCLUDED
