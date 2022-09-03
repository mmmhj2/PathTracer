#ifndef PDF_MIXTURE_H_INCLUDED
#define PDF_MIXTURE_H_INCLUDED

#include "pdf_base.h"
#include <memory>

class pdf_mixture : public pdf_base
{
    std::shared_ptr <pdf_base> p1, p2;
public:
    pdf_mixture() = delete;
    pdf_mixture(std::shared_ptr <pdf_base> _p1, std::shared_ptr <pdf_base> _p2)
    : p1(_p1), p2(_p2) {};

    virtual double prob(const vec3 & direction, const onb & uvw) const override
    {
        return (p1->prob(direction, uvw) + p2->prob(direction, uvw)) * 0.5;
    }

    virtual vec3 sample(const onb & uvw) const override
    {
        return uvw.local(tools::random_double() < 0.5 ? p1->sample(uvw) : p2->sample(uvw));
    }
};

class pdf_mixture_weighted : public pdf_base
{
    std::shared_ptr <pdf_base> p1, p2;
    double w;
public:
    pdf_mixture_weighted() = delete;
    pdf_mixture_weighted(std::shared_ptr <pdf_base> _p1, std::shared_ptr <pdf_base> _p2, double _w)
    : p1(_p1), p2(_p2), w(_w) {};

    virtual double prob(const vec3 & direction, const onb & uvw) const override
    {
        return (w * p1->prob(direction, uvw) + (1-w) * p2->prob(direction, uvw));
    }

    virtual vec3 sample(const onb & uvw) const override
    {
        return uvw.local(tools::random_double() < w ? p1->sample(uvw) : p2->sample(uvw));
    }
};

#endif // PDF_MIXTURE_H_INCLUDED
