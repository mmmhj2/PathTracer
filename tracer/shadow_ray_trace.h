#ifndef SHADOW_RAY_TRACE_H_INCLUDED
#define SHADOW_RAY_TRACE_H_INCLUDED

#include "objects/objlist_base.h"
#include "material/pdf/pdf_object.h"

bool gen_shadow_ray(const hit_record & rec, std::vector <std::shared_ptr<light_base>> * lights, light_sample & sample)
{
    if(lights->empty())
        return false;

    int light_sz = (int)lights->size();
    int rindex = tools::random_int(0, light_sz - 1);

    sample = (*lights)[rindex]->sample_Li(rec);
    if(sample.is_occluded)
        return false;
    if(sample.pdf * 10000 < 1)
        return false;

    return true;
}

#endif // SHADOW_RAY_TRACE_H_INCLUDED
