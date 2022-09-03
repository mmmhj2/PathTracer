#ifndef LIGHT_SHAPE_H_INCLUDED
#define LIGHT_SHAPE_H_INCLUDED

#include "light_base.h"

#include <iostream>

class diffuse_light_shape : public light_base
{
    std::shared_ptr <hittable_object> ptr;
public:

    diffuse_light_shape() = delete;
    diffuse_light_shape(std::shared_ptr<objlist_base> _world, std::shared_ptr <hittable_object> _ptr)
    : light_base(_world), ptr(_ptr) {}

    virtual light_sample sample_Li(const hit_record & rec) const
    {
        vec3 sampled_point = ptr->sample();

        light_sample ret;
        ret.clr = color(0, 0, 0);
        ret.shadow_ray = ray(rec.p, sampled_point - rec.p);

        // Test occlusion
        hit_record world_rec;
        if(!ptr->hit(ret.shadow_ray, 0.001, constants::dinf, ret.shadow_hitrec))
        {
            ret.is_occluded = true;
            return ret;
        }

        if(world->hit(ret.shadow_ray, 0.001, ret.shadow_hitrec.t - 1e-7, world_rec))
        {
            ret.is_occluded = true;
            return ret;
        }

        ret.is_occluded = false;
        ret.pdf = ptr->pdf_value(ret.shadow_ray, ret.shadow_hitrec);
        return ret;
    }

    virtual double pdf_Li(const ray & shadow_ray, const hit_record & rec) const
    {
        return ptr->pdf_value(shadow_ray, rec);
    }
};

#endif // LIGHT_SHAPE_H_INCLUDED
