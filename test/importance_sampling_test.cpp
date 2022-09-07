#include "ray/light/light.h"
#include "objects/triangle_flat.h"
#include "objects/spatial_structure/bvh/bvh.h"

#include "material/lambertian_simple.h"
#include "material/emissive_simple.h"
#include "material/metallic.h"
#include "material/texture/texture_base.h"
#include "material/texture/checker_texture.h"
#include "material/texture/image_texture.h"
#include "objects/triangle_flat.h"
#include "objects/sphere.h"
#include "objects/spatial_structure/bvh/bvh.h"
#include "ray/camera.h"
#include "skybox/skybox_colored.h"
#include "tracer/tracer.h"
#include "image/ImageOutputPPM.h"

#include "cornellbox_def.h"

#include <iostream>
#include <fstream>
#include <future>

using namespace std;

int main()
{
    using namespace cornell_box;
    sobol_generator::init_generator();
    camera cam(point3(278,278,1350), point3(278,278,0), vec3(0,1,0), constants::pi / 4.5, 1.0);
    skybox_color sky;
    auto world = make_shared<bvh_tree>();

    auto red_tex   = make_shared<solid_color>(color(.65, .05, .05));
    auto white_tex = make_shared<solid_color>(color(.73, .73, .73));
    auto light_tex = make_shared<solid_color>(color(25, 25, 25));
    auto green_tex = make_shared<solid_color>(color(.12, .45, .15));
    auto back_tex = make_shared<image_texture>("checker-map_tho.png");
    auto red   = make_shared<lambertian>(red_tex);
    auto white = make_shared<lambertian>(white_tex);
    auto green = make_shared<lambertian>(green_tex);
    auto back_mat = make_shared<lambertian>(back_tex);

    auto sphere_mat = make_shared<metallic>(white_tex);

    auto light = make_shared<emissive_diffuse>(light_tex);
    auto light_t1 = make_shared<triangle_flat>(triangle_vert(vert_up_light), triangle_uv(uv), light);
    auto light_t2 = make_shared<triangle_flat>(triangle_vert(vert_up_light+2), triangle_uv(uv+2), light);
    auto light_l1 = make_shared<diffuse_light_area>(world, light_t1);
    auto light_l2 = make_shared<diffuse_light_area>(world, light_t2);

    world->add_object(make_shared<triangle_flat>(triangle_vert(vert_right), triangle_uv(uv), green));
    world->add_object(make_shared<triangle_flat>(triangle_vert(vert_right+2), triangle_uv(uv+2), green));
    world->add_object(make_shared<triangle_flat>(triangle_vert(vert_left), triangle_uv(uv), red));
    world->add_object(make_shared<triangle_flat>(triangle_vert(vert_left+2), triangle_uv(uv+2), red));
    world->add_object(make_shared<triangle_flat>(triangle_vert(vert_back), triangle_uv(uv), back_mat));
    world->add_object(make_shared<triangle_flat>(triangle_vert(vert_back+2), triangle_uv(uv+2), back_mat));
    world->add_object(make_shared<triangle_flat>(triangle_vert(vert_down), triangle_uv(uv), white));
    world->add_object(make_shared<triangle_flat>(triangle_vert(vert_down+2), triangle_uv(uv+2), white));
    world->add_object(make_shared<triangle_flat>(triangle_vert(vert_up), triangle_uv(uv), white));
    world->add_object(make_shared<triangle_flat>(triangle_vert(vert_up+2), triangle_uv(uv+2), white));
    world->add_object(light_t1);
    world->add_object(light_t2);
    world->add_object(make_shared<sphere>(point3( 270, 100, 270), 100, sphere_mat));
    world->build();
    light_list lights;
    lights.push_back(light_l1);
    lights.push_back(light_l2);

    std::vector <concurrent::block_info> infos(constants::blocks);


    int image_width = 600;
    int image_height = 600;

    {
        concurrent::fill_info(infos, image_width, image_height, constants::max_depth, constants::blocks, &cam, world.get(), &sky, &lights);

        std::vector <std::future<std::vector<color>>> async_task;
        for(int i = 0; i < constants::blocks; i++)
            async_task.push_back(std::async(concurrent::trace_block, std::cref(infos[i])));

        pbar::init_curses();
        while(!pbar::print_progressbar(infos))
        {
            std::this_thread::sleep_for(100ms);
        }
        pbar::decon_curses();

        std::vector <color> pic;
        for(int i = 0; i < constants::blocks; i++)
        {
            std::vector <color> block = async_task[i].get();
            pic.insert(pic.end(), block.cbegin(), block.cend());
        }


        ofstream fout;
        fout.open("importance_sampling.ppm");
        ImageOutputPPM()(image_width, image_height, fout, pic);
        fout.close();
    }
    // Generate Albedo map for OIDN
    {
        concurrent::fill_info(infos, image_width, image_height, 1, constants::blocks, &cam, world.get(), &sky, nullptr);

        std::vector <std::future<std::vector<color>>> async_task;
        for(int i = 0; i < constants::blocks; i++)
            async_task.push_back(std::async(concurrent::albedo_trace_block, std::cref(infos[i])));

        pbar::init_curses();
        while(!pbar::print_progressbar(infos))
        {
            std::this_thread::sleep_for(100ms);
        }
        pbar::decon_curses();

        std::vector <color> pic;
        for(int i = 0; i < constants::blocks; i++)
        {
            std::vector <color> block = async_task[i].get();
            pic.insert(pic.end(), block.cbegin(), block.cend());
        }


        ofstream fout;
        fout.open("importance_sampling_albedo.ppm");
        ImageOutputPPM()(image_width, image_height, fout, pic);
        fout.close();
    }

}
