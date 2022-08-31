#ifndef MATERIAL_BASE_H_INCLUDED
#define MATERIAL_BASE_H_INCLUDED

#include "ray/ray.h"
#include "utils/color.h"
#include <memory>

struct hit_record;

class material
{
public:

    virtual bool evaluateEmissive(const ray & incident,
                          const hit_record & rec,
                          color & emissive) const = 0;

    virtual bool evaluateScatter(const ray & incident,
                                 const hit_record & rec,
                                 color & attenuation,
                                 ray & scattered) const = 0;
};

class non_emissive_material : public material
{
public:
    virtual bool evaluateEmissive([[maybe_unused]] const ray & incident,
                                  [[maybe_unused]] const hit_record & rec,
                                  [[maybe_unused]] color & emissive) const override
    {
        return false;
    }
};

#endif // MATERIAL_BASE_H_INCLUDED
