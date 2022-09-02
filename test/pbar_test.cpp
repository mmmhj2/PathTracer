#include <iostream>
#include <fstream>
#include <future>
//#define TRIANGLE_TEST
#include "material/lambertian_simple.h"
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

using namespace std;

int main()
{
    camera cam(point3(278,278,1500), point3(278,278,0), vec3(0,1,0), constants::pi / 4.5, 1.0);
    skybox_color_gradient sky;
    bvh_tree world;

    vec3 vert_right[] =
    {
        vec3(0, 0, 555),
        vec3(0, 0, 0),
        vec3(0, 555, 0),
        vec3(0, 555, 555),
        vec3(0, 0, 555)
    };
    vec3 vert_left[] =
    {
        vec3(555, 0, 555),
        vec3(555, 0, 0),
        vec3(555, 555, 0),
        vec3(555, 555, 555),
        vec3(555, 0, 555)
    };
    vec3 vert_back[] =
    {
        vec3(0, 0, 0),
        vec3(555, 0, 0),
        vec3(555, 555, 0),
        vec3(0, 555, 0),
        vec3(0, 0, 0)
    };
    vec3 vert_down[] =
    {
        vec3(0, 0, 555),
        vec3(555, 0, 555),
        vec3(555, 0, 0),
        vec3(0, 0, 0),
        vec3(0, 0, 555)
    };

    std::pair <double, double> uv[5] =
    {
        std::make_pair(0.0, 0.0),
        std::make_pair(1.0, 0.0),
        std::make_pair(1.0, 1.0),
        std::make_pair(0.0, 1.0),
        std::make_pair(0.0, 0.0)
    };

    auto red_tex   = make_shared<solid_color>(color(.65, .05, .05));
    auto white_tex = make_shared<solid_color>(color(.73, .73, .73));
    auto green_tex = make_shared<solid_color>(color(.12, .45, .15));
    auto back_tex = make_shared<image_texture>("back.jpg");
    auto red   = make_shared<lambertian>(red_tex);
    auto white = make_shared<lambertian>(white_tex);
    auto green = make_shared<lambertian>(green_tex);
    auto back_mat = make_shared<lambertian>(back_tex);

    world.add_object(make_shared<triangle_flat>(vert_right, uv, green));
    world.add_object(make_shared<triangle_flat>(vert_right+2, uv+2, green));
    world.add_object(make_shared<triangle_flat>(vert_left, uv, red));
    world.add_object(make_shared<triangle_flat>(vert_left+2, uv+2, red));
    world.add_object(make_shared<triangle_flat>(vert_back, uv, back_mat));
    world.add_object(make_shared<triangle_flat>(vert_back+2, uv+2, back_mat));
    world.add_object(make_shared<triangle_flat>(vert_down, uv, white));
    world.add_object(make_shared<triangle_flat>(vert_down+2, uv+2, white));

    world.build();

    std::vector <concurrent::block_info> infos(constants::blocks);
    int image_width = 600;
    int image_height = 600;

    concurrent::fill_info(infos, image_width, image_height, constants::blocks, &cam, &world, &sky);

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
    fout.open("pbar.ppm");
    ImageOutputPPM()(image_width, image_height, fout, pic);
    fout.close();
}
