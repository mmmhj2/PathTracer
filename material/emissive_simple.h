#ifndef EMISSIVE_SIMPLE_H_INCLUDED
#define EMISSIVE_SIMPLE_H_INCLUDED

#include "material_base.h"
#include "objects/object_base.h"
#include "texture/texture_base.h"

class emissive_diffuse : public material
{
private:
    std::shared_ptr <uv_texture> emit;

public:
    emissive_diffuse() = delete;
    emissive_diffuse(std::shared_ptr <uv_texture> _emit)
    : emit(_emit) {};

    virtual bool evaluateEmissive(const ray & incident,
                          const hit_record & rec,
                          color & emissive) const
    {
        // Introducing a hard-coded cosine PDF
        emissive = emit->get_color(rec.u, rec.v, rec.p);
        double cosine = std::abs(incident.direction() * rec.normal / (incident.direction().norm() * rec.normal.norm()));
        emissive = emissive * cosine;
        return true;
    }

    virtual bool evaluateScatter(const ray & incident,
                                 const hit_record & rec,
                                 color & attenuation,
                                 ray & scattered) const
    {
        return false;
    }
};

#endif // EMISSIVE_SIMPLE_H_INCLUDED
