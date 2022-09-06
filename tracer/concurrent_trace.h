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
    int max_depth;
    camera * cam;
    objlist_base * world;
    skybox_base * skybox;
    light_list * lights;

    mutable std::atomic_int progress;
};

void fill_info(std::vector<block_info> & infos,
               int width, int height, int max_depth, int blocks,
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
        infos[i].max_depth = max_depth;
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

    auto mat_ptr = rec.obj->get_material();
    std::shared_ptr <BSDF_base> emissive_bsdf, scatter_bsdf;

    color emissive;
    color b_sample, l_sample;
    light_sample smpl;


    bool is_emissive = mat_ptr->evaluateEmissive(r, rec, emissive_bsdf);
    if(is_emissive)
        emissive = emissive_bsdf->eval(r, r);
    if(!mat_ptr->evaluateScatter(r, rec, scatter_bsdf))
        return emissive;

    scatter_bsdf->sample(r, scattered);
    b_sample = elem_product(scatter_bsdf->eval(r, scattered), ray_trace(scattered, world, skybox, lights, depth-1));

    // No MIS if no lights or scatter BSDF contains delta function
    // PDF of light source would be zero
    if(lights == nullptr || scatter_bsdf->is_delta_bsdf())
        return emissive + b_sample;

    l_sample = trace_shadow_ray(r, rec, world, skybox, lights);

    // Do not use heuristic weight if light contains delta function
    if(smpl->light->is_delta_light())
        return emissive + b_sample / 2 + l_sample / 2;

    // Use heuristic power technique introduced at
    // https://www.pbr-book.org/3ed-2018/Monte_Carlo_Integration/Importance_Sampling
    /*double ls_l_pdf, ls_b_pdf, bs_l_pdf, bs_b_pdf;

    ls_l_pdf = smpl.pdf;
    ls_b_pdf = scatter_bsdf->pdf(r, smpl.shadow_ray);
    bs_l_pdf = smpl.light->pdf_Li(scattered, rec);
    bs_b_pdf = scatter_bsdf->pdf(r, scattered);

    double weight_l, weight_b;
    weight_l = heuristic_weight(1, ls_l_pdf, 1, ls_b_pdf);
    weight_b = heuristic_weight(1, bs_b_pdf, 1, bs_l_pdf);*/

    return emissive + b_sample * (.5) + l_sample * (.5);
}

color albedo_trace(const ray & r, const objlist_base & world, const skybox_base & skybox)
{
    hit_record rec;
    if(!world.hit(r, 0.001, constants::dinf, rec))
        return skybox(r);

    auto mat_ptr = rec.obj->get_material();
    return mat_ptr->getAlbedo(rec);
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
                result += ray_trace(r, *(info.world), *(info.skybox), info.lights, info.max_depth);
            }
            result /= constants::sample_per_pixel;
            ret.push_back(result);
        }
        info.progress++;
    }

    return ret;
}

std::vector <color> albedo_trace_block(const block_info & info)
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
                result += albedo_trace(r, *(info.world), *(info.skybox));
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
