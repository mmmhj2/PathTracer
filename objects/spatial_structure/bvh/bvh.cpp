#include "bvh.h"
#include "utils/defs.h"

bvh_node::bvh_node(obj_vec & objs, size_t start, size_t fin)
{
    int axis = tools::random_int(0, 2);
    auto comparator_shared = [axis](const obj_ptr & l, const obj_ptr & r)
    {
        return aabb::compare_box_axis(l.get(), r.get(), axis);
    };

    size_t span = fin - start;

    if(span == 1)
    {
        lch = rch = objs[start];
    }
    else if(span == 2)
    {
        if(comparator_shared(objs[start], objs[start+1]))
            lch = objs[start], rch = objs[start+1];
        else
            lch = objs[start+1], rch = objs[start];
    }
    else
    {
        std::sort(objs.begin() + start, objs.begin() + fin, comparator_shared);
        size_t mid = start + span >> 1;

        lch = std::make_shared<bvh_node>(objs, start, mid);
        rch = std::make_shared<bvh_node>(objs, mid, fin);
    }

    // Construct bounding box for this node
    aabb box_l, box_r;

    if(!lch->get_aabb(box_l) || !rch->get_aabb(box_r))
        ;

    this->box = aabb::surrounding_box(box_l, box_r);
}


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

bool bvh_tree::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
    if(root != nullptr)
        return root->hit(r, t_min, t_max, rec);
    return false;
}

void bvh_tree::add_object(std::shared_ptr<hittable_object>obj)
{
    this->objs.push_back(obj);
}

bool bvh_tree::get_aabb(aabb& output) const
{
    if(root != nullptr)
        return root->get_aabb(output);
    return false;
}

void bvh_tree::build()
{
    root = std::make_shared<bvh_node>(objs, 0, objs.size());
}

