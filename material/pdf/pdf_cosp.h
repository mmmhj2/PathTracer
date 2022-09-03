#ifndef PDF_COSP_H_INCLUDED
#define PDF_COSP_H_INCLUDED

#include "pdf_base.h"

// Cosine powered PDF, used to sample glossy term of Blinn-Phong BSDF
// Note : it samples half vector instead of incident vector
class pdf_cos_power : public pdf_base
{
    int n;
    onb uvw;
    vec3 omega_out;
public:
    pdf_cos_power() = delete;
    pdf_cos_power(int power, const vec3 & normal, const vec3 & out)
    : n(power), uvw(normal), omega_out((-out).unit())
    {}

    virtual double prob(const vec3 & half) const override
    {
        double cosine = half * uvw[2];
        if(cosine < 0)
            return 0;
        double cosine_power = std::pow(cosine, n);
        double woDotwh = (omega_out * half.unit());
        return (n+1) / (2*constants::pi) * cosine_power / woDotwh;
    }

    virtual vec3 sample_hemisphere() const override
    {
        double theta, phi, u1, u2;
        u1 = tools::random_double();
        u2 = tools::random_double();
        theta = std::acos(std::pow(u1, 1 / (n+1)));
        phi = u2 * 2 * constants::pi;

        return vec3(std::cos(phi) * std::sin(theta), std::sin(phi) * std::sin(theta), std::cos(theta));
    }

};

#endif // PDF_COSP_H_INCLUDED
