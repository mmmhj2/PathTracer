#ifndef VEC3D_H_INCLUDED
#define VEC3D_H_INCLUDED

#include "vec_template.h"

typedef vec_t <double, 3> vec3d;
typedef vec_t <float, 3> vec3f;
typedef vec_t <int, 3> vec3i;

// Use double for default type
typedef vec3d vec3;
typedef vec3 point3;

// TODO : partial specialization for vec3 to speed up
template<>
vec_t<double, 3>::vec_t()
{
    d[0] = d[1] = d[2] = 0;
}
template<>
vec_t<float, 3>::vec_t()
{
    d[0] = d[1] = d[2] = 0;
}
template<>
vec_t<int, 3>::vec_t()
{
    d[0] = d[1] = d[2] = 0;
}

#endif // VEC3D_H_INCLUDED
