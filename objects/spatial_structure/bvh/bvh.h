#ifndef BVH_H_INCLUDED
#define BVH_H_INCLUDED

#include "../../objlist_base.h"

class bvh_node : public hittable_object
{
private:

    std::shared_ptr <hittable_object> lch, rch;
    aabb box;

public:
    virtual bool hit(const ray & r, double t_min, double t_max, hit_record & rec) const override;
    virtual bool get_aabb(aabb & output) const override;
};

class bvh_tree : public objlist_base
{

};

#endif // BVH_H_INCLUDED
