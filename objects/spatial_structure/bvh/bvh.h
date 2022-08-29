#ifndef BVH_H_INCLUDED
#define BVH_H_INCLUDED

#include "../../objlist_base.h"

class bvh_node : public hittable_object
{
public:
    typedef std::shared_ptr <hittable_object> obj_ptr;
    typedef std::vector <obj_ptr> obj_vec;

private:

    obj_ptr lch, rch;
    aabb box;

public:
    bvh_node(obj_vec & objs, size_t start, size_t fin);
    virtual bool hit(const ray & r, double t_min, double t_max, hit_record & rec) const override;
    virtual bool get_aabb(aabb & output) const override;
};

class bvh_tree : public objlist_base
{
private:
    std::vector <std::shared_ptr<hittable_object>> objs;
    std::shared_ptr <bvh_node> root;

public:
    virtual bool hit(const ray & r, double t_min, double t_max, hit_record & rec) const override;
    virtual void add_object(std::shared_ptr <hittable_object> obj) override;
    virtual bool get_aabb(aabb & output) const override;
    virtual void build();

};

#endif // BVH_H_INCLUDED
