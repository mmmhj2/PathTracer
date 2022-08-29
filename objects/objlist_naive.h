#ifndef OBJLIST_NAIVE_H_INCLUDED
#define OBJLIST_NAIVE_H_INCLUDED

#include <list>
#include <memory>
#include "object_base.h"
#include "objlist_base.h"

class objlist_naive : public objlist_base
{
public:
    virtual bool hit(const ray & r, double t_min, double t_max, hit_record & rec) const override;
    virtual void add_object(std::shared_ptr <hittable_object> obj) override;
    virtual bool get_aabb(aabb & output) const override;
    virtual void build(){};
private:
    std::list<std::shared_ptr<hittable_object>> objlist;
};

bool objlist_naive::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
    hit_record trec;
    bool anyhit = false;
    double closest = t_max;

    for(const auto & obj : objlist)
    {
        if(obj->hit(r, t_min, closest, trec))
        {
            anyhit = true;
            closest = trec.t;
            rec = trec;
        }
    }

    return anyhit;
}

void objlist_naive::add_object(std::shared_ptr<hittable_object>obj)
{
    objlist.push_back(obj);
}

bool objlist_naive::get_aabb(aabb& output) const
{
    if(objlist.empty())
        return false;

    aabb temp_box;
    bool is_first_box = true;

    for(const auto & obj : objlist)
    {
        if(!obj->get_aabb(temp_box))
            return false;

        if(is_first_box)
            output = temp_box, is_first_box = false;
        else
            output = aabb::surrounding_box(output, temp_box);
    }

    return true;
}


#endif // OBJLIST_NAIVE_H_INCLUDED
