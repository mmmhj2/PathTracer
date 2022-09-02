#ifndef CONCURRENT_TRACE_H_INCLUDED
#define CONCURRENT_TRACE_H_INCLUDED

#include <vector>
#include <atomic>
#include "utils/vec3d.h"
#include "utils/defs.h"
#include "ray/ray.h"
#include "objects/objlist_base.h"
#include "material/material_base.h"

namespace concurrent
{

struct block_info
{
    int scanline_max;
    int scanline_min;
    int image_height, image_width;
    camera * cam;
    objlist_base * world;
    skybox_base * skybox;

    mutable std::atomic_int progress;
};

color ray_trace(const ray & r, const objlist_base & world, const skybox_base & skybox, int depth)
{
    if(depth <= 0)
        return color(0, 0, 0);

    hit_record rec;

    if(!world.hit(r, 0.001, constants::dinf, rec))
        return skybox(r);

    ray scattered;
    color attenu, emissive;
    auto mat_ptr = rec.mat.lock();

    mat_ptr->evaluateEmissive(r, rec, emissive);
    if(mat_ptr->evaluateScatter(r, rec, attenu, scattered))
        return emissive + elem_product(attenu, ray_trace(scattered, world, skybox, depth - 1));
    return emissive;
}

std::vector <color> trace_block(const block_info & info)
{
    std::vector <color> ret;
    for(int j = info.scanline_max - 1; j >= info.scanline_min; j--)
    {
        for(int i = 0; i < info.image_width; i++)
        {
            color result;
            for(int k = 0; k < constants::sample_per_pixel; k++)
            {
                double u, v;
                u = (i + tools::random_double()) / (info.image_width - 1);
                v = (j + tools::random_double()) / (info.image_height - 1);
                ray r = info.cam->get_ray(u, v);
                result += ray_trace(r, *(info.world), *(info.skybox), 20);
            }
            result /= constants::sample_per_pixel;
            ret.push_back(result);
        }
        info.progress++;
    }

    return ret;
}

}

#endif // CONCURRENT_TRACE_H_INCLUDED
