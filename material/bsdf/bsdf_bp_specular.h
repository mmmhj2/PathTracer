#ifndef BSDF_BP_SPECULAR_H_INCLUDED
#define BSDF_BP_SPECULAR_H_INCLUDED

#include "bsdf_base.h"
#include "utils/qrng/sobol_qrng.h"

// BSDF used for specular term of Blinn-Phong BSDF
class BSDF_BP_specular : public BSDF_base
{
    int power;
    std::shared_ptr <uv_texture> spec;

    //vec3 half_vec;
public:
    BSDF_BP_specular(const hit_record & rec, int _power, std::shared_ptr<uv_texture> tex)
    : BSDF_base(rec), power(_power), spec(tex) {};

    virtual color eval_raw(const ray & o, const ray & i) const
    {
        vec3 omega_out = -o.direction().unit();
        vec3 omega_in = i.direction().unit();
        vec3 half_vec = (omega_out + omega_in).unit();
        double z = std::max(0., rec.normal.unit() * half_vec);
        double weight = (power + 2.0) / (2 * constants::pi) * std::pow(z, power * 1.0);
        return spec->get_color(rec.u, rec.v, rec.p) * weight * (omega_in * rec.normal.unit());
    }

    virtual color eval(const ray & o, const ray & i) const
    {
        vec3 omega_out = -o.direction().unit();
        vec3 half_vec = (omega_out + i.direction().unit()).unit();
        double weight = (power + 2.0) / (power + 1.0) * std::max(0., omega_out * half_vec);
        return spec->get_color(rec.u, rec.v, rec.p) * weight;
    }

    virtual double pdf(const ray & o, const ray & i) const override
    {
        vec3 omega_out = -o.direction().unit();
        vec3 half_vec = (omega_out + i.direction().unit()).unit();
        double cos_beta = half_vec * rec.normal.unit();

        return (power + 1.0) * std::pow(cos_beta, power * 1.0) / (8 * constants::pi * (omega_out * half_vec));
    }

    virtual void sample(const ray & o, ray & spl) const override
    {
        double r1, r2;
//        r1 = tools::random_double();
//        r2 = tools::random_double();
        std::tie(r1, r2) = sobol_generator::random_double_pair();
        double theta_half, phi_half;
        theta_half = std::acos(std::pow(r1, 1.0 / (power + 1)));
        phi_half = 2 * constants::pi * r2;

        double x, y, z;
        onb uvw(rec.normal);
        x = std::cos(phi_half) * std::sin(theta_half);
        y = std::sin(phi_half) * std::sin(theta_half);
        z = std::cos(theta_half);
        vec3 half_vec = uvw.local(vec3(x, y, z));

        // Generate incident ray from half vector
        vec3 idir = 2.0 * half_vec + o.direction().unit();
        spl = ray(rec.p, idir);
    }
};

#endif // BSDF_BP_SPECULAR_H_INCLUDED
