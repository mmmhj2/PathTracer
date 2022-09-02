#include <iostream>
#include <fstream>
#include <future>
//#define TRIANGLE_TEST
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

using namespace std;

void test_simple()
{
    auto tex = std::make_shared<solid_color>(color(1, 1, 0));
    auto mat = std::make_shared<lambertian>(tex);

    vec3 vert[3] =
    {
        vec3(0, 0, 0),
        vec3(1, 0, 0),
        vec3(0, 1, 0)
    };

    std::pair <double, double> uv[3] =
    {
        std::make_pair(0.0, 0.0),
        std::make_pair(1.0, 0.0),
        std::make_pair(0.0, 1.0)
    };

    auto triangle = std::make_shared<triangle_flat>(vert, uv, mat);
    aabb tri_aabb;
    triangle->get_aabb(tri_aabb);

    cout << "AABB : " << tri_aabb.min() << " " << tri_aabb.max() << endl ;


    for(int i = 0; i < 50; i++)
    {
        double _x, _y;
        _x = tools::random_double();
        _y = tools::random_double();
        ray test_ray(point3(_x, _y, 1), vec3(0, 0, -1));
        hit_record test_record;


        bool ishit = triangle->hit(test_ray, 0, constants::dinf, test_record);

        cout << "====================" << endl ;
        cout << "Ray from " << test_ray.origin() << endl ;
        cout << "Hit ? " << ishit << endl ;
        if(!ishit)
            continue;
        cout << "Hit at : " << test_record.p << endl ;
        cout << "Local normal : " << test_record.normal << endl ;
        cout << "UV : " << test_record.u << ","  << test_record.v << endl ;
    }
}

int main()
{
    using namespace cornell_box;
    camera cam(point3(278,278,1350), point3(278,278,0), vec3(0,1,0), constants::pi / 4.5, 1.0);
    skybox_color sky;
    bvh_tree world;

    auto red_tex   = make_shared<solid_color>(color(.65, .05, .05));
    auto white_tex = make_shared<solid_color>(color(.73, .73, .73));
    auto light_tex = make_shared<solid_color>(color(50., 50., 50.));
    auto green_tex = make_shared<solid_color>(color(.12, .45, .15));
    auto back_tex = make_shared<image_texture>("checker-map_tho.png");
    auto red   = make_shared<lambertian>(red_tex);
    auto white = make_shared<lambertian>(white_tex);
    auto green = make_shared<lambertian>(green_tex);
    auto back_mat = make_shared<lambertian>(back_tex);
    auto light = make_shared<emissive_diffuse>(light_tex);

    world.add_object(make_shared<triangle_flat>(vert_right, uv, green));
    world.add_object(make_shared<triangle_flat>(vert_right+2, uv+2, green));
    world.add_object(make_shared<triangle_flat>(vert_left, uv, red));
    world.add_object(make_shared<triangle_flat>(vert_left+2, uv+2, red));
    world.add_object(make_shared<triangle_flat>(vert_back, uv, back_mat));
    world.add_object(make_shared<triangle_flat>(vert_back+2, uv+2, back_mat));
    world.add_object(make_shared<triangle_flat>(vert_down, uv, white));
    world.add_object(make_shared<triangle_flat>(vert_down+2, uv+2, white));
    world.add_object(make_shared<triangle_flat>(vert_up, uv, white));
    world.add_object(make_shared<triangle_flat>(vert_up+2, uv+2, white));
    world.add_object(make_shared<triangle_flat>(vert_up_light, uv, light));
    world.add_object(make_shared<triangle_flat>(vert_up_light+2, uv+2, light));
    //world.add_object(make_shared<sphere>(point3( 0.0, 0.0, 0.0), 200, white));
    world.build();

    std::vector <concurrent::block_info> infos(constants::blocks);
    int image_width = 600;
    int image_height = 600;

    concurrent::fill_info(infos, image_width, image_height, constants::blocks, &cam, &world, &sky);

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
    fout.open("triangle.ppm");
    ImageOutputPPM()(image_width, image_height, fout, pic);
    fout.close();
}
