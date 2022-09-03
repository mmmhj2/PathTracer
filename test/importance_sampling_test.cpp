#include "ray/light/light.h"
#include "objects/triangle_flat.h"
#include "objects/spatial_structure/bvh/bvh.h"

#include "material/lambertian_simple.h"
#include "material/emissive_simple.h"

#include <iostream>

using namespace std;

vec3 vert1[3] =
{
    vec3(0, 0, 0),
    vec3(1, 0, 0),
    vec3(0, 1, 0)
};

vec3 vert2[3] =
{
    vec3(0, 0, 0.5),
    vec3(1, 0, 0.5),
    vec3(0, 1, 0.5)
};

std::pair <double, double> uv[3] =
{
    std::make_pair(0, 0),
    std::make_pair(1, 0),
    std::make_pair(0, 1)
};

int main()
{
    auto tex = std::make_shared<solid_color>(color(1, 0, 0));
    auto mat = std::make_shared<lambertian>(tex);

    auto triangle = std::make_shared<triangle_flat>(vert1, uv, mat);
    auto triangle_occ = std::make_shared<triangle_flat>(vert2, uv, mat);
    auto world = std::make_shared<bvh_tree>();
    auto light = std::make_shared<diffuse_light_shape>(world, triangle);

    hit_record test_rec;
    test_rec.p = point3(0, 0, 1);
    test_rec.normal = point3(0, 0, -1);

    world->add_object(triangle);
    world->build();

    for(int i = 0; i < 50; i++)
    {
        light_sample smp = light->sample_Li(test_rec);
        cout << smp.is_occluded << " " << smp.shadow_ray.origin() << " " << smp.shadow_ray.direction() << " " << smp.pdf << endl ; ;
    }


    world->add_object(triangle_occ);
    world->build();

    for(int i = 0; i < 50; i++)
    {
        light_sample smp = light->sample_Li(test_rec);
        cout << smp.is_occluded << " " << smp.shadow_ray.origin() << " " << smp.shadow_ray.direction() << " " << smp.pdf << endl ; ;
    }
}
