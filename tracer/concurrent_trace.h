#ifndef CONCURRENT_TRACE_H_INCLUDED
#define CONCURRENT_TRACE_H_INCLUDED

#include <vector>
#include <atomic>
#include "utils/vec3d.h"
#include "utils/defs.h"
#include "utils/qrng/sobol_qrng.h"
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

double heuristic_weight(int nf, double fPdf, int ng, double gPdf)
{
    double f, g;
    f = nf * fPdf, g = ng * gPdf;
    return (f * f) / (f * f + g * g);
//    return f / (f + g);
}

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

    auto mat_ptr = rec.mat.lock();
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
    if(lights == nullptr || scatter_bsdf->is_delta_bsdf())
        return emissive + b_sample;

    color light_color;
    gen_shadow_ray(rec, lights, smpl);

    if(smpl.cannot_hit)
        return emissive + b_sample;

    if(smpl.is_occluded)
    {
        light_color = ray_trace(smpl.shadow_ray, world, skybox, nullptr, depth-1);
        l_sample = elem_product(scatter_bsdf->eval_raw(r, smpl.shadow_ray), light_color);
        l_sample /= smpl.pdf;
        return emissive + b_sample / 2 + l_sample / 2;
    }

    trace_shadow_ray(smpl, light_color);
    l_sample = elem_product(scatter_bsdf->eval_raw(r, smpl.shadow_ray), light_color);

    //if(trace_shadow_ray(smpl, light_color))
    //    l_sample = elem_product(scatter_bsdf->eval(r, smpl.shadow_ray), light_color);

    // Use heuristic power technique introduced at
    // https://www.pbr-book.org/3ed-2018/Monte_Carlo_Integration/Importance_Sampling
    double light_pdf, bsdf_pdf, weight;
    light_pdf = smpl.pdf, bsdf_pdf = scatter_bsdf->pdf(r, scattered);
    weight = heuristic_weight(1, bsdf_pdf, 1, light_pdf);

    return emissive + b_sample * (weight) + l_sample * (1 - weight);
}

color albedo_trace(const ray & r, const objlist_base & world, const skybox_base & skybox)
{
    hit_record rec;
    if(!world.hit(r, 0.001, constants::dinf, rec))
        return skybox(r);

    auto mat_ptr = rec.mat.lock();
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
                std::tie(u, v) = sobol_generator::random_double_pair();
                u = (i + u - 0.5) / (info.image_width - 1);
                v = (j + v - 0.5) / (info.image_height - 1);
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
