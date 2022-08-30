#ifndef CHECKER_TEXTURE_H_INCLUDED
#define CHECKER_TEXTURE_H_INCLUDED

#include "texture_base.h"

class checker_texture : public uv_texture
{
private:
    color even, odd;
    double size_coef;
public:
    checker_texture () = delete;
    checker_texture (color e, color o, double s = 10)
    : even(e), odd(o), size_coef(s)
    {
    }

    virtual color get_color(double u, double v, const point3 & p) const override
    {
        auto sines = std::sin(size_coef * p[0]) * std::sin(size_coef * p[1]) * std::sin(size_coef * p[2]);

        if(sines < 0)
            return odd;
        else
            return even;
    }
};

#endif // CHECKER_TEXTURE_H_INCLUDED
