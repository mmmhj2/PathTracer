#ifndef PDF_BASE_H_INCLUDED
#define PDF_BASE_H_INCLUDED

#include "utils/orthonormal_basis.h"
#include "utils/defs.h"

// Base class for probability density function
class pdf_base
{
public:

    virtual double prob(const vec3 & direction) const = 0;
    virtual vec3 sample_hemisphere() const = 0;
};

class pdf_uniform : public pdf_base
{
    onb uvw;
public:
    pdf_uniform () = delete;
    pdf_uniform (const vec3 & normal) : uvw(normal)
    {
    }

    virtual double prob(const vec3 & direction) const override
    {
        double cosine = direction * uvw[2];
        return (cosine > 0 ? 1.0 / constants::pi : 0);
    }

    virtual vec3 sample_hemisphere() const override
    {
        using constants::pi;
        double r1, r2;
        r1 = tools::random_double();
        r2 = tools::random_double();

        double x, y, z;
        x = std::cos(2 * pi * r1) * 2 * std::sqrt(r2 * (1-r2));
        y = std::sin(2 * pi * r1) * 2 * std::sqrt(r2 * (1-r2));
        z = 1 - 2 * r2;

        return vec3(x, y, (z > 0 ? z : -z));
    }
};

class pdf_cosine : public pdf_base
{
    onb uvw;
public:
    pdf_cosine () = delete;
    pdf_cosine (const vec3 & normal) : uvw(normal)
    {
    }

    virtual double prob(const vec3 & direction) const override
    {
        double cosine = direction * uvw[2];
        return (cosine > 0 ? cosine / constants::pi : 0);
    }

    virtual vec3 sample_hemisphere() const override
    {
        double r1, r2, z, phi;
        r1 = tools::random_double();
        r2 = tools::random_double();
        z = std::sqrt(1 - r2);
        phi = 2 * constants::pi * r1;

        return vec3(std::cos(phi) * std::sqrt(r2), std::sin(phi) * std::sqrt(r2), z);
    }
};

#endif // PDF_BASE_H_INCLUDED
