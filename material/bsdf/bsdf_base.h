#ifndef BSDF_BASE_H_INCLUDED
#define BSDF_BASE_H_INCLUDED

#include "ray/ray.h"
#include "objects/object_base.h"
#include "utils/color.h"

struct BSDF_Sample
{
    ray in_rev;
    double pdf;
};

class BSDF_base
{
protected:

    hit_record rec;

public:

    BSDF_base() = delete;
    BSDF_base(const hit_record & _rec) : rec(_rec) {};

    virtual color eval(const ray & o, const ray & i) const = 0;
    virtual double pdf(const ray & o, const ray & i) const = 0;
    virtual void sample(const ray & o, BSDF_Sample & sample) const  = 0;

};

#endif // BSDF_BASE_H_INCLUDED
