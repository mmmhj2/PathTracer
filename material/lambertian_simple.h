#ifndef LAMBERTIAN_SIMPLE_H_INCLUDED
#define LAMBERTIAN_SIMPLE_H_INCLUDED

#include "material_base.h"

class lambertian : public material
{
public:
    lambertian(color clr) : k_L(std::move(clr))
    {
    }

    virtual bool evaluateScatter(const ray & incident,
                                 const hit_record & rec,
                                 color & attenuation,
                                 ray & scattered);
private:
    color k_L;
};

bool lambertian::evaluateScatter(const ray& incident, const hit_record& rec, color& attenuation, ray& scattered)
{
    auto scatter_dir = rec.normal + vec3::random_in_sphere();
    scattered = ray(rec.p, scatter_dir);
    attenuation = k_L;
    return true;
}


#endif // LAMBERTIAN_SIMPLE_H_INCLUDED
