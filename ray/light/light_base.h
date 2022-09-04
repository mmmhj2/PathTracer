#ifndef LIGHT_BASE_H_INCLUDED
#define LIGHT_BASE_H_INCLUDED

#include "objects/object_base.h"
#include "objects/objlist_base.h"
#include "../ray.h"
#include "utils/color.h"

struct light_sample
{
    hit_record shadow_hitrec;
    ray shadow_ray;

    double pdf;
    bool cannot_hit, is_occluded, is_delta_light;
};

class light_base
{
protected:

    std::shared_ptr <objlist_base> world;

public:
    light_base() = delete;
    light_base(std::shared_ptr <objlist_base> _world)
    : world(_world) {}

    virtual light_sample sample_Li(const hit_record & rec) const = 0;
    virtual double pdf_Li(const ray & shadow_ray, const hit_record & rec) const = 0;
};

#endif // LIGHT_BASE_H_INCLUDED
