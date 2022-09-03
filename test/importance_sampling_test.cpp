#include "ray/light/light.h"
#include "objects/triangle_flat.h"
#include "objects/spatial_structure/bvh/bvh.h"

#include "material/lambertian_simple.h"
#include "material/emissive_simple.h"
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

    auto light = make_shared<emissive_diffuse>(light_tex);
    auto light_t1 = make_shared<triangle_flat>(vert_up_light, uv, light);
    auto light_t2 = make_shared<triangle_flat>(vert_up_light+2, uv+2, light);
    auto light_l1 = make_shared<diffuse_light_shape>(world, light_t1);
    auto light_l2 = make_shared<diffuse_light_shape>(world, light_t2);

    world->add_object(make_shared<triangle_flat>(vert_right, uv, green));
    world->add_object(make_shared<triangle_flat>(vert_right+2, uv+2, green));
    world->add_object(make_shared<triangle_flat>(vert_left, uv, red));
    world->add_object(make_shared<triangle_flat>(vert_left+2, uv+2, red));
    world->add_object(make_shared<triangle_flat>(vert_back, uv, back_mat));
    world->add_object(make_shared<triangle_flat>(vert_back+2, uv+2, back_mat));
    world->add_object(make_shared<triangle_flat>(vert_down, uv, white));
    world->add_object(make_shared<triangle_flat>(vert_down+2, uv+2, white));
    world->add_object(make_shared<triangle_flat>(vert_up, uv, white));
    world->add_object(make_shared<triangle_flat>(vert_up+2, uv+2, white));
    world->add_object(light_t1);
    world->add_object(light_t2);
    //world.add_object(make_shared<sphere>(point3( 0.0, 0.0, 0.0), 200, white));
    world->build();
    light_list lights;
    lights.push_back(light_l1);
    lights.push_back(light_l2);

    std::vector <concurrent::block_info> infos(constants::blocks);



    int image_width = 600;
    int image_height = 600;

    concurrent::fill_info(infos, image_width, image_height, constants::blocks, &cam, world.get(), &sky, &lights);

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
