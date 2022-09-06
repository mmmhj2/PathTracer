#ifndef LIGHT_SHAPE_H_INCLUDED
#define LIGHT_SHAPE_H_INCLUDED

#include "light_base.h"
#include "material/material_base.h"
#include "material/bsdf/bsdf.h"

#include <iostream>

class diffuse_light_area : public light_base
{
    std::shared_ptr <hittable_object> ptr;
public:

    diffuse_light_area() = delete;
    diffuse_light_area(std::shared_ptr<objlist_base> _world, std::shared_ptr <hittable_object> _ptr)
    : light_base(_world), ptr(_ptr) {}

    virtual light_sample sample_Li(const hit_record & rec) const
    {
        vec3 sampled_vec = ptr->sample(rec.p);

        light_sample ret;
        ret.shadow_ray = ray(rec.p, sampled_vec);
        ret.is_delta_light = false;
        ret.light = this;

        // Test occlusion
        hit_record world_rec;
        if(!ptr->hit(ret.shadow_ray, 0.001, constants::dinf, ret.shadow_hitrec))
        {
            ret.pdf = 0;
            ret.cannot_hit = true;
            return ret;
        }
        ret.cannot_hit = false;

        ret.pdf = ptr->pdf_value(ret.shadow_ray, ret.shadow_hitrec);
        if(this->is_occluded(ret.shadow_ray, ret.shadow_hitrec))
        {
            ret.is_occluded = true;
            return ret;
        }

        ret.is_occluded = false;
        return ret;
    }

    virtual color evaluate(const hit_record & from, const ray & direction_out) const
    {
        std::shared_ptr <BSDF_base> emissive_bsdf;
        ptr->get_material()->evaluateEmissive(direction_out, from, emissive_bsdf);
        return emissive_bsdf->eval(direction_out, direction_out);
    }

    virtual double pdf_Li(const ray & shadow_ray) const
    {
        hit_record rec;
        if(ptr->hit(shadow_ray, 0.0001, constants::dinf, rec))
            return this->pdf_Li(shadow_ray,rec);
        return 0;
    }

    virtual double pdf_Li(const ray & shadow_ray, const hit_record & rec) const
    {
        return ptr->pdf_value(shadow_ray, rec);
    }

    virtual bool is_samplable(const ray & r) const
    {
        hit_record rec;
        if(ptr->hit(r, 0.001, constants::dinf, rec))
            return true;
        return false;
    }

    virtual bool is_occluded(const ray & r, const hit_record & hit_this_light) const
    {
        hit_record world_rec;
        if(world->hit(r, 0.001, hit_this_light.t - 1e-7, world_rec))
            return true;
        return false;
    }
};

#endif // LIGHT_SHAPE_H_INCLUDED
