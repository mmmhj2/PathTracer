#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

#include "vec3d.h"
#include "defs.h"
#include <tuple>

typedef vec3 color;

std::tuple<short, short, short> color_to_RGB(const color & pixel)
{
    double r = pixel[0];
    double g = pixel[1];
    double b = pixel[2];

    // Gamma correction using gamma=2.0
    r = std::sqrt(r);
    g = std::sqrt(g);
    b = std::sqrt(b);

    return std::make_tuple(static_cast<int>(256 * tools::clamp(r, 0.0, 0.99)),
                           static_cast<int>(256 * tools::clamp(g, 0.0, 0.99)),
                           static_cast<int>(256 * tools::clamp(b, 0.0, 0.99))
                           );
}

#endif // COLOR_H_INCLUDED
