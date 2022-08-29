#ifndef METALLIC_H_INCLUDED
#define METALLIC_H_INCLUDED

#include "material_base.h"

class metallic : public material
{
private:
    color albedo;

public:
    metallic(color alb) : albedo(alb)
    {
    }

    virtual bool evaluateScatter(const ray & incident, const hit_record & rec, color & attenuation, ray & scattered) const override;
};

bool metallic::evaluateScatter(const ray& incident, const hit_record& rec, color& attenuation, ray& scattered) const
{
    vec3 reflected = tools::reflect(incident.direction().unit(), rec.normal);
    scattered = ray(rec.p, reflected);
    attenuation = albedo;
    return (scattered.direction() * rec.normal > 0);
}


#endif // METALLIC_H_INCLUDED
