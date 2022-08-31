#ifndef SKYBOX_COLORED_H_INCLUDED
#define SKYBOX_COLORED_H_INCLUDED

#include "skybox_base.h"

class skybox_color_gradient : public skybox_base
{
public:
    virtual color operator () (const ray & r) const
    {
        auto unit = r.dir.unit();
        auto t = 0.5 * (unit[1] + 1.0);
        return (1.0 - t) * color(1, 1, 1) + t * color(0.5, 0.7, 1.0);
    }
};

class skybox_color : public skybox_base
{
    color sky_color;
public:
    skybox_color() : sky_color(color(0, 0, 0))
    {
    }

    skybox_color(color _clr) : sky_color(_clr)
    {
    }

    virtual color operator () ([[maybe_unused]] const ray & r) const
    {
        return sky_color;
    }
};

#endif // SKYBOX_COLORED_H_INCLUDED
