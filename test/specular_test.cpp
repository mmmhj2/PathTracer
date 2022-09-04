#include <iostream>
#include <fstream>
#include <future>

#include "utils/defs.h"
#include "image/ImageOutputPPM.h"
#include "skybox/skybox_colored.h"
#include "objects/sphere.h"
#include "objects/spatial_structure/bvh/bvh.h"
#include "ray/camera.h"
#include "tracer/tracer.h"
#include "material/lambertian_simple.h"
#include "material/dielectric.h"
#include "material/metallic.h"
#include "material/texture/checker_texture.h"
using namespace std;

int main()
{
    camera cam(point3(-2,2,1), point3(0,0,-1), vec3(0,1,0), constants::pi / 9);
    skybox_color_gradient sky;
    bvh_tree world;

    auto texture_ground = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    auto texture_center = make_shared<solid_color>(color(0.1, 0.2, 0.5));
    auto texture_full_white = make_shared<solid_color>(color(1, 1, 1));

    auto material_ground = make_shared<lambertian>(texture_ground);
    auto material_center = make_shared<lambertian>(texture_center);
    auto material_left   = make_shared<dielectric>(1.5, texture_full_white);
    auto material_right  = make_shared<metallic>(texture_full_white);

    world.add_object(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add_object(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add_object(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add_object(make_shared<sphere>(point3(-1.0,    0.0, -1.0), -0.45, material_left));
    world.add_object(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    world.build();

    std::vector <concurrent::block_info> infos(constants::blocks);
    int image_width = 600;
    int image_height = image_width / constants::aspect_ratio;

    concurrent::fill_info(infos, image_width, image_height, constants::blocks, &cam, &world, &sky, nullptr);

    std::vector <std::future<std::vector<color>>> async_task;
    for(int i = 0; i < constants::blocks; i++)
        async_task.push_back(std::async(concurrent::trace_block, std::cref(infos[i])));

    pbar::init_curses();
    while(true)
    {
        if(pbar::print_progressbar(infos))
            break;
        std::this_thread::sleep_for(200ms);
    }
    pbar::decon_curses();
    cout << endl ;

    std::vector <color> pic;
    for(int i = 0; i < constants::blocks; i++)
    {
        std::vector <color> block = async_task[i].get();
        pic.insert(pic.end(), block.cbegin(), block.cend());
    }


    ofstream fout;
    fout.open("specular.ppm");
    ImageOutputPPM()(image_width, image_height, fout, pic);
    fout.close();
}
