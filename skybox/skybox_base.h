#ifndef SKYBOX_BASE_H_INCLUDED
#define SKYBOX_BASE_H_INCLUDED

#include "utils/color.h"
#include "ray/ray.h"

class skybox_base
{
public:
    virtual color operator () (const ray & r) const = 0;
};

#endif // SKYBOX_BASE_H_INCLUDED
