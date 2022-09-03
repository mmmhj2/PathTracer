#ifndef OBJLIST_BASE_H_INCLUDED
#define OBJLIST_BASE_H_INCLUDED

#include "ray/ray.h"
#include "object_base.h"

class objlist_base : public hittable_object
{
public:

    //virtual ~objlist_base() = 0;

    virtual void add_object(std::shared_ptr <hittable_object> obj) = 0;
    virtual void build() = 0;
    // virtual bool hit(const ray & r, double t_min, double t_max, hit_record & rec) const = 0;

    //virtual double pdf_value(const ray & r, const hit_record & h) const = 0;
    //virtual vec3 sample() const = 0;

};

#endif // OBJLIST_BASE_H_INCLUDED
