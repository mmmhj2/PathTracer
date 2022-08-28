#ifndef MATERIAL_BASE_H_INCLUDED
#define MATERIAL_BASE_H_INCLUDED

#include "../ray/ray.h"
#include "../utils/color.h"

class material
{
public:
    virtual bool evaluateScatter(const ray & incident,
                                 const hit_record & rec,
                                 color & attenuation,
                                 ray & scattered) const = 0;
};

#endif // MATERIAL_BASE_H_INCLUDED
