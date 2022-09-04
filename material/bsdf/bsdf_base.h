#ifndef BSDF_BASE_H_INCLUDED
#define BSDF_BASE_H_INCLUDED

#include "ray/ray.h"
#include "objects/object_base.h"
#include "utils/color.h"

class BSDF_base
{
protected:

    hit_record rec;

public:

    BSDF_base() = delete;
    BSDF_base(const hit_record & _rec) : rec(_rec) {};

    virtual color eval(const ray & o, const ray & i) const = 0;
    virtual double pdf(const ray & o, const ray & i) const = 0;
    virtual void sample(const ray & o, ray & sample) const = 0;

    // If a BSDF contains Dirac delta function
    // Its Monte Carlo integration needs to be handled carefully
    // Especially when using Multiple Importance Sampling
    virtual bool is_delta_bsdf() const
    {
        return false;
    }
};

class BSDF_delta_base : public BSDF_base
{
public:
    BSDF_delta_base(const hit_record & rec)
    : BSDF_base(rec) {};

    virtual double pdf(const ray &, const ray &) const
    {
        return 0;
    }

    virtual bool is_delta_bsdf() const
    {
        return true;
    }
};

#endif // BSDF_BASE_H_INCLUDED
