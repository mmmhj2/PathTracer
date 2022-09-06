#ifndef TRIANGLE_FLAT_H_INCLUDED
#define TRIANGLE_FLAT_H_INCLUDED

#include "object_base.h"
#include "material/material_base.h"
#include "spatial_structure/mesh.h"

class triangle_flat : public hittable_object
{
private:
    constexpr static double parallel_eps = 1e-6;

    //point3 * vert;
    //uv_pair * uv;

    triangle_vert vert;
    triangle_uv uv;

    point3 normal;

    double area;

    int flag;
    std::shared_ptr <material> mat;

public:

    constexpr static int FLAG_BIDIRECTIONAL = 0x0000;
    constexpr static int FLAG_DIRECTIONAL = 0x0001;
    constexpr static int FLAG_INVERSENORMAL = 0x0002;

    triangle_flat(const triangle_vert & _vert,
                  const triangle_uv & _uv,
                  std::shared_ptr <material> _mat,
                  int _flag = FLAG_DIRECTIONAL)
        : vert(_vert), uv(_uv)
    {
        //std::copy(_vert, _vert+3, vert);
        //std::copy(_uv, _uv+3, uv);
        // Flat triangle
        normal = ((*(vert[1]) - *(vert[0])) ^ (*(vert[2]) - *(vert[0])));
        mat = _mat;

        flag = _flag;
        if(flag & FLAG_INVERSENORMAL)
            normal = -normal;
        area = normal.norm() / 2.0;
    }

    virtual std::shared_ptr <material> get_material() const
    {
        return mat;
    }

    virtual bool hit (const ray & r, double t_min, double t_max, hit_record & rec) const
    {
        // Parallel test
        double NdotRay = normal * r.direction();
        if(-parallel_eps <= NdotRay && NdotRay <= parallel_eps)
            return false;

        double t = - (normal * r.origin() - normal * (*(vert[0]))) / NdotRay;
        if(t < t_min || t > t_max)
            return false;

        vec3 p = r.origin() + t * r.direction();

        // Inside-outside test

        double area_square = normal * normal;
        double barycentric[3];

        for(int i = 0; i < 3; i++)
        {
            vec3 edge = *(vert[(i+1) % 3]) - *(vert[i]);
            vec3 vp = p - *(vert[i]);
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

        if(flag & FLAG_DIRECTIONAL && rec.is_front == false)
            return false;

        rec.t = t;
        rec.obj = this;
        rec.u = barycentric[1] * uv[0]->first + barycentric[2] * uv[1]->first + barycentric[0] * uv[2]->first;
        rec.v = barycentric[1] * uv[0]->second + barycentric[2] * uv[1]->second + barycentric[0] * uv[2]->second;
        return true;
    }

    virtual bool get_aabb(aabb & output) const
    {
        vec3d _max, _min;

        for(int i = 0; i < 3; i++)
        {
            _max[i] = std::max({(*vert[0])[i], (*vert[1])[i], (*vert[2])[i]});
            _min[i] = std::min({(*vert[0])[i], (*vert[1])[i], (*vert[2])[i]});

            // Extend AABB a little
            if(_max[i] - _min[i] <= 0.001)
                _max[i] += 0.001, _min[i] -= 0.001;
        }
        output = aabb(_min, _max);
        return true;
    }

    virtual point3 get_centroid() const
    {
        return (*vert[0] + *vert[1] + *vert[2]) / 3.0;
    }

    virtual double pdf_value(const ray & r, const hit_record & h) const
    {
        double distance_sq = h.t * h.t * r.direction().norm_squared();
        double cosine = std::abs((r.direction() * h.normal) / (r.direction().norm() * h.normal.norm()));

        return distance_sq / (cosine * area);
    }

    virtual vec3 sample(const point3 & from) const
    {
        double r1, r2;
        r1 = tools::random_double();
        r2 = tools::random_double();
        vec3 rand_point = (1 - std::sqrt(r1)) * *vert[0] + (std::sqrt(r1)*(1 - r2)) * *vert[1] + (r2*std::sqrt(r1)) * *vert[2];
        return rand_point - from;
    }

};

#endif // TRIANGLE_FLAT_H_INCLUDED
