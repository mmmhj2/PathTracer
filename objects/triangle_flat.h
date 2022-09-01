#ifndef TRIANGLE_FLAT_H_INCLUDED
#define TRIANGLE_FLAT_H_INCLUDED

#include "object_base.h"
#include "material/material_base.h"

class triangle_flat : public hittable_object
{
private:
    constexpr static double parallel_eps = 1e-6;
    typedef std::pair <double, double> uv_pair;

    point3 vert[3];
    uv_pair uv[3];
    point3 normal;

    std::shared_ptr <material> mat;

public:

    triangle_flat(point3 _vert[3], uv_pair _uv[3], std::shared_ptr <material> _mat)
    {
        std::copy(_vert, _vert+3, vert);
        std::copy(_uv, _uv+3, uv);
        // Flat triangle
        normal = ((vert[1] - vert[0]) ^ (vert[2] - vert[0]));
        mat = _mat;
    }

    virtual bool hit (const ray & r, double t_min, double t_max, hit_record & rec) const
    {
        // Parallel test
        double NdotRay = normal * r.direction();
        if(-parallel_eps <= NdotRay && NdotRay <= parallel_eps)
            return false;

        double t = - (normal * r.origin() - normal * vert[0]) / NdotRay;
        if(t < t_min || t > t_max)
            return false;

        vec3 p = r.origin() + t * r.direction();

        // Inside-outside test

        double area_square = normal * normal;
        double barycentric[3];

        for(int i = 0; i < 3; i++)
        {
            vec3 edge = vert[(i+1) % 3] - vert[i];
            vec3 vp = p - vert[i];
            vec3 C = edge ^ vp;
            barycentric[i] = normal * C / area_square;
            if(barycentric[i] < 0)
                return false;
        }
#ifdef TRIANGLE_TEST
        std::cerr << "barycentric coordinates : " << barycentric[0] << " " << barycentric[1] << " " << barycentric[2] << std::endl ;
#endif
        rec.p = p;
        rec.set_normal(r, normal.unit());
        rec.t = t;
        rec.mat = mat;
        rec.u = barycentric[1] * uv[0].first + barycentric[2] * uv[1].first + barycentric[0] * uv[2].first;
        rec.v = barycentric[1] * uv[0].second + barycentric[2] * uv[1].second + barycentric[0] * uv[2].second;
        return true;
    }

    virtual bool get_aabb(aabb & output) const
    {
        vec3d _max, _min;

        for(int i = 0; i < 3; i++)
        {
            _max[i] = std::max({vert[0][i], vert[1][i], vert[2][i]});
            _min[i] = std::min({vert[0][i], vert[1][i], vert[2][i]});

            // Extend AABB a little
            if(_max[i] - _min[i] <= 0.001)
                _max[i] += 0.001, _min[i] -= 0.001;
        }
        output = aabb(_min, _max);
        return true;
    }

    virtual point3 get_centroid() const
    {
        return (vert[0] + vert[1] + vert[2]) / 3.0;
    }

};

#endif // TRIANGLE_FLAT_H_INCLUDED
