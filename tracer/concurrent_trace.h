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

#include "shadow_ray_trace.h"

struct block_info
{
    int scanline_max;
    int scanline_min;
    int image_height, image_width;
    camera * cam;
    objlist_base * world;
    skybox_base * skybox;
    light_list * lights;

    mutable std::atomic_int progress;
};

void fill_info(std::vector<block_info> & infos,
               int width, int height, int blocks,
               camera * cam,
               objlist_base * world,
               skybox_base * sky,
               light_list * lights)
{
    int scanline_per_blocks = height / blocks + 1;
    for(int i = 0; i < blocks; i++)
    {
        infos[i].scanline_max = height - i * scanline_per_blocks;
        const int scanline_min = height - (i+1) * scanline_per_blocks;
        infos[i].scanline_min = std::max(0, scanline_min);
        infos[i].image_height = height;
        infos[i].image_width = width;
#ifdef TRACER_TEST
        cout << "Block " << i << " from " << infos[i].scanline_max << " to " << infos[i].scanline_min << endl ;
#endif
        infos[i].cam = cam;
        infos[i].skybox = sky;
        infos[i].world = world;
        infos[i].lights = lights;
        infos[i].progress = 0;
    }
}

color ray_trace(const ray & r, const objlist_base & world, const skybox_base & skybox, light_list * lights, int depth)
{
    if(depth <= 0)
        return color(0, 0, 0);

    hit_record rec;

    if(!world.hit(r, 0.001, constants::dinf, rec))
        return skybox(r);

    ray scattered;
    color attenu, emissive;
    auto mat_ptr = rec.mat.lock();

    // Evaluate self-emitting
    mat_ptr->evaluateEmissive(r, rec, emissive);

    color scatter_color = color(0, 0, 0);
    color shadow_ray_color = color(0, 0, 0);

    if(mat_ptr->evaluateScatter(r, rec, attenu, scattered))
    {
        scatter_color = elem_product(attenu, ray_trace(scattered, world, skybox, lights, depth - 1));
        shadow_ray_color = gen_shadow_ray(rec, lights);
    }

    return emissive + scatter_color + shadow_ray_color;
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
                result += ray_trace(r, *(info.world), *(info.skybox), info.lights, 20);
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
