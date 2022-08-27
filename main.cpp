#include <fstream>
#include <iostream>
#include "utils/defs.h"
#include "image/ImageOutputPPM.h"
#include "ray/skybox_base.h"
#include "objects/sphere.h"
#include "objects/objlist_naive.h"
#include "ray/camera.h"

#include <future>

using namespace std;

color ray_trace(const ray & r, const objlist_naive & world, const skybox_base & skybox, int depth)
{
    if(depth <= 0)
        return color(0, 0, 0);

    hit_record rec;

    if(world.hit(r, 0, constants::dinf, rec))
    {
        point3 target = rec.p + rec.normal + vec3::random_in_sphere() ;
        return 0.5 * ray_trace(ray(rec.p, target - rec.p), world, skybox, depth-1);
    }

    return skybox(r);
}

int main()
{
    skybox_base sky;
    std::vector <color> pic;

    objlist_naive world;
    world.push_back(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.push_back(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

    camera cam;

    for(int j = constants::image_height - 1; j >= 0; j--)
    {
        for(int i = 0; i < constants::image_width; i++)
        {
            std::vector <std::future<color>> async_tasks;
            for(int k = 0; k < constants::sample_per_pixel; k++)
            {
                double u, v;
                u = (i + tools::random_double()) / (constants::image_width - 1);
                v = (j + tools::random_double()) / (constants::image_height - 1);
                ray r = cam.get_ray(u, v);
                async_tasks.push_back(std::async(ray_trace, r, world, sky, 20));
            }
            color result;
            for(auto & future : async_tasks)
                result += future.get();
            result /= constants::sample_per_pixel;

            pic.push_back(result);
        }
        cerr << "Completed " << constants::image_height - j << " scanlines" << endl;
    }

    ofstream fout;
    fout.open("result.ppm");
    ImageOutputPPM()(constants::image_width, constants::image_height, fout, pic);
    fout.close();

    ray r;

    return 0;
}
