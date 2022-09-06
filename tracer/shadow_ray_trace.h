#ifndef SHADOW_RAY_TRACE_H_INCLUDED
#define SHADOW_RAY_TRACE_H_INCLUDED

#include "objects/objlist_base.h"
//#include "material/pdf/pdf_object.h"
#include "ray/light/light.h"

bool gen_shadow_ray(const hit_record & rec, std::vector <std::shared_ptr<light_base>> * lights, light_sample & sample)
{
    if(lights->empty())
        return false;

    int light_sz = (int)lights->size();
    int rindex = tools::random_int(0, light_sz - 1);

    sample = (*lights)[rindex]->sample_Li(rec);
    if(sample.cannot_hit || sample.is_occluded)
        return false;
    if(sample.pdf * 10000 < 1)
        return false;

    return true;
}

bool trace_shadow_ray(const light_sample & sample, color & emissive)
{
    if(sample.is_occluded)
        return false;
    std::shared_ptr <BSDF_base> bsdf;
    bool is_emissive =
        sample.shadow_hitrec.mat.lock()->evaluateEmissive(sample.shadow_ray, sample.shadow_hitrec, bsdf);
    emissive = bsdf->eval(sample.shadow_ray, sample.shadow_ray) / sample.pdf;
    return is_emissive;
}

color trace_shadow_ray_recursive(const ray & r)
{

}

#endif // SHADOW_RAY_TRACE_H_INCLUDED
