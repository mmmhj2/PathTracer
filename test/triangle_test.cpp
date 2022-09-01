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
#include "utils/concurrent_trace.h"
#include "image/ImageOutputPPM.h"

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
    //world.add_object(make_shared<sphere>(point3( 0.0, 0.0, 0.0), 200, white));
    world.build();

    std::vector <concurrent::block_info> infos(constants::blocks);
    int image_width = 600;
    int image_height = 600;

    int scanline_per_blocks = image_height / constants::blocks + 1;
    for(int i = 0; i < constants::blocks; i++)
    {
        infos[i].scanline_max = image_height - i * scanline_per_blocks;
        const int scanline_min = image_height - (i+1) * scanline_per_blocks;
        infos[i].scanline_min = std::max(0, scanline_min);
        infos[i].image_height = image_height;
        infos[i].image_width = image_width;
        cout << "Block " << i << " from " << infos[i].scanline_max << " to " << infos[i].scanline_min << endl ;
        infos[i].cam = &cam;
        infos[i].skybox = &sky;
        infos[i].world = &world;
        infos[i].progress = 0;
    }

    std::vector <std::future<std::vector<color>>> async_task;
    for(int i = 0; i < constants::blocks; i++)
        async_task.push_back(std::async(concurrent::trace_block, std::cref(infos[i])));

    while(true)
    {
        bool completed = true;

        for(int i = 0; i < constants::blocks; i++)
        {
            cout << "(" << infos[i].progress << " of " << infos[i].scanline_max - infos[i].scanline_min << ")";
            if(infos[i].progress < infos[i].scanline_max - infos[i].scanline_min)
                completed = false;
        }
        cout << endl ;

        if(completed)
            break;

        std::this_thread::sleep_for(100ms);
    }
    cout << endl ;

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
