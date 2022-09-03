#ifndef PDF_OBJECT_H_INCLUDED
#define PDF_OBJECT_H_INCLUDED

#include "objects/object_base.h"
#include "pdf_base.h"

// PDF wrapper to call the PDF of a hittable object
class pdf_object : public pdf_base
{
private:
    point3 o;
    std::shared_ptr <hittable_object> ptr;

public:
    pdf_object() = delete;
    pdf_object(const point3 & origin, std::shared_ptr <hittable_object> obj)
    : o(origin), ptr(obj) {};

    virtual double prob(const vec3 & direction, const onb & uvw) const override
    {
        return ptr->pdf_value(o, direction);
    }

    virtual vec3 sample(const onb & uvw) const override
    {
        return ptr->sample() - o;
    }
};

#endif // PDF_OBJECT_H_INCLUDED
