#ifndef MATERIAL_BASE_H_INCLUDED
#define MATERIAL_BASE_H_INCLUDED

#include "bsdf/bsdf.h"
#include "ray/ray.h"
#include "utils/color.h"
#include <memory>

struct hit_record;

class material
{
public:

    virtual bool evaluateEmissive(const ray & incident,
                          const hit_record & rec,
                          std::shared_ptr <BSDF_base> & bsdf) const = 0;

    virtual bool evaluateScatter(const ray & incident,
                                 const hit_record & rec,
                                 std::shared_ptr <BSDF_base> & bsdf) const = 0;
};

class non_emissive_material : public material
{
public:
    virtual bool evaluateEmissive([[maybe_unused]] const ray & incident,
                                  [[maybe_unused]] const hit_record & rec,
                                  [[maybe_unused]] std::shared_ptr <BSDF_base> & bsdf) const override
    {
        return false;
    }
};

#endif // MATERIAL_BASE_H_INCLUDED
