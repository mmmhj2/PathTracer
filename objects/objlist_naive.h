#ifndef OBJLIST_NAIVE_H_INCLUDED
#define OBJLIST_NAIVE_H_INCLUDED

#include <list>
#include <memory>
#include "object_base.h"

class objlist_naive : public std::list<std::shared_ptr<hittable_object>>
{
public:
    bool hit(const ray & r, double t_min, double t_max, hit_record & rec) const;
};

bool objlist_naive::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
    hit_record trec;
    bool anyhit = false;
    double closest = t_max;

    for(const auto & obj : *this)
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


#endif // OBJLIST_NAIVE_H_INCLUDED
