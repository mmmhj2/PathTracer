#ifndef SHADOW_RAY_TRACE_H_INCLUDED
#define SHADOW_RAY_TRACE_H_INCLUDED

#include "objects/objlist_base.h"
#include "material/pdf/pdf_object.h"

color gen_shadow_ray(const hit_record & rec, std::vector <std::shared_ptr<light_base>> * lights)
{
    int light_sz = (int)lights->size();
    int rindex = tools::random_int(0, light_sz - 1);

    light_sample smp = (*lights)[rindex]->sample_Li(rec);
    smp.pdf /= light_sz;

    color emissive;
    smp.shadow_hitrec.mat.lock()->evaluateEmissive(smp.shadow_ray, smp.shadow_hitrec, emissive);
    return emissive / smp.pdf;
}

#endif // SHADOW_RAY_TRACE_H_INCLUDED
