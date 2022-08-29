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

#endif // VEC3D_H_INCLUDED
