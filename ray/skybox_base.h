#ifndef SKYBOX_BASE_H_INCLUDED
#define SKYBOX_BASE_H_INCLUDED

#include "../utils/color.h"
#include "ray.h"

class skybox_base
{
public:
    virtual color operator () (const ray & r)
    {
        auto unit = r.dir.unit();
        auto t = 0.5 * (unit[1] + 1.0);
        return (1.0 - t) * color(1, 1, 1) + t * color(0.5, 0.7, 1.0);
    }
};

#endif // SKYBOX_BASE_H_INCLUDED
