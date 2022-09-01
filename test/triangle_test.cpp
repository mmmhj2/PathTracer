#include <iostream>
#include <fstream>
#include <future>
//#define TRIANGLE_TEST
#include "material/lambertian_simple.h"
#include "material/texture/texture_base.h"
#include "material/texture/checker_texture.h"
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
    camera cam(point3(-2,2,1), point3(0,0,-1), vec3(0,1,0), constants::pi / 9);
    skybox_color_gradient sky;
    bvh_tree world;

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

    auto texture_ground = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    //auto texture_up = make_shared<solid_color>(color(20, 20, 20));
    auto texture_center = make_shared<solid_color>(color(0.4, 0, 0));

    auto material_ground = make_shared<lambertian>(texture_ground);
    //auto material_up = make_shared<emissive_diffuse>(texture_up);
    //auto material_center = make_shared<blinn_phong_naive>(color(0.4, 0, 0), color(1, 1, 1), 10);
    auto material_center = make_shared<lambertian>(texture_center);

    world.add_object(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    //world.add_object(make_shared<sphere>(point3( 0.0,    1.0, -1.0),   0.25, material_up));
    world.add_object(make_shared<triangle_flat>(vert, uv, material_center));
    world.build();

    std::vector <concurrent::block_info> infos(constants::blocks);
    int scanline_per_blocks = constants::image_height / constants::blocks + 1;
    for(int i = 0; i < constants::blocks; i++)
    {
        infos[i].scanline_max = constants::image_height - i * scanline_per_blocks;
        const int scanline_min = constants::image_height - (i+1) * scanline_per_blocks;
        infos[i].scanline_min = std::max(0, scanline_min);
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
    ImageOutputPPM()(constants::image_width, constants::image_height, fout, pic);
    fout.close();
}
