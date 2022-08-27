#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

#include "vec3d.h"
#include <tuple>

typedef vec3d color;

std::tuple<short, short, short> color_to_RGB(const color & pixel)
{
    return std::make_tuple(static_cast<int>(255.999 * pixel[0]),
                           static_cast<int>(255.999 * pixel[1]),
                           static_cast<int>(255.999 * pixel[2])
                           );
}

#endif // COLOR_H_INCLUDED
