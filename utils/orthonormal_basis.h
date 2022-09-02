#ifndef ORTHONORMAL_BASIS_H_INCLUDED
#define ORTHONORMAL_BASIS_H_INCLUDED

#include "vec3d.h"

class orthonormal_basis
{
    vec3 axis[3];
public:
    orthonormal_basis() = delete;

    orthonormal_basis(const vec3 & normal)
    {
        axis[2] = normal.unit();
        vec3 a = ((std::abs(axis[2][0]) > 0.9) ? vec3(0, 1, 0) : vec3(1, 0, 0));
        axis[1] = (axis[2] ^ a).unit();
        axis[0] = axis[2] ^ axis[1];
    }

    inline vec3 & operator[] (int i) noexcept
    {
        return axis[i];
    }

    inline const vec3 & operator[] (int i) const noexcept
    {
        return axis[i];
    }

    vec3 local(const vec3 & a) const noexcept
    {
        return a[0] * axis[0] + a[1] * axis[1] + a[2] * axis[2];
    }

};

typedef orthonormal_basis onb;

#endif // ORTHONORMAL_BASIS_H_INCLUDED
