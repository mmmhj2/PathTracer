#include "bvh.h"

bool bvh_node::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
    if(!box.hit(r, t_min, t_max))
        return false;
    bool hit_l, hit_r;
    hit_l = lch->hit(r, t_min, t_max, rec);
    hit_r = rch->hit(r, t_min, hit_l ? rec.t : t_max, rec);

    return hit_l || hit_r;
}

bool bvh_node::get_aabb(aabb& output) const
{
    output = this->box;
    return true;
}

