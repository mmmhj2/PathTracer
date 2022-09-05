#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include "utils/vec3d.h"

typedef std::pair <double, double> uv_pair;

struct triangle_vert
{
    const point3 * v1;
    const point3 * v2;
    const point3 * v3;

    explicit triangle_vert(const point3 * v)
    {
        v1 = v;
        v2 = v+1;
        v3 = v+2;
    }

    triangle_vert(const triangle_vert &) = default;
    triangle_vert(triangle_vert &&) = default;

    const point3 * operator [] (int index) const noexcept
    {
        switch(index)
        {
        case 0:
            return v1;
        case 1:
            return v2;
        case 2:
            return v3;
        }
        return v1;
    }
};

struct triangle_uv
{
    const uv_pair * u1;
    const uv_pair * u2;
    const uv_pair * u3;

    explicit triangle_uv(const uv_pair * u)
    {
        u1 = u;
        u2 = u+1;
        u3 = u+2;
    }

    triangle_uv(const triangle_uv &) = default;
    triangle_uv(triangle_uv &&) = default;

    const uv_pair * operator [] (int index) const noexcept
    {
        switch(index)
        {
        case 0:
            return u1;
        case 1:
            return u2;
        case 2:
            return u3;
        }
        return u1;
    }
};

struct triangle_normal
{
    const vec3 * n1;
    const vec3 * n2;
    const vec3 * n3;

    triangle_normal(const triangle_normal &) = default;
    triangle_normal(triangle_normal &&) = default;

    const vec3 * operator [] (int index) const noexcept
    {
        switch(index)
        {
        case 0:
            return n1;
        case 1:
            return n2;
        case 2:
            return n3;
        }
        return n1;
    }
};

#endif // MESH_H_INCLUDED
