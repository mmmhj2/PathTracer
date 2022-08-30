#ifndef TEXTURE_BASE_H_INCLUDED
#define TEXTURE_BASE_H_INCLUDED

#include "utils/defs.h"
#include "utils/color.h"

class uv_texture
{
public:
    virtual color get_color(double u, double v, const point3 & p) const = 0;
};

class solid_color : public uv_texture
{
private:
    color color_value;

public:
    solid_color () = delete;
    solid_color (color clr)
    : color_value(clr) {};

    virtual color get_color([[maybe_unused]] double u,
                            [[maybe_unused]] double v,
                            [[maybe_unused]] const point3 & p) const
    {
        return color_value;
    }
};

#endif // TEXTURE_BASE_H_INCLUDED
