#include <fstream>
#include <iostream>
#include "utils/defs.h"
#include "image/ImageOutputPPM.h"
#include "ray/skybox_base.h"
#include "objects/sphere.h"
#include "objects/objlist_naive.h"
#include "ray/camera.h"
using namespace std;

color ray_trace(const ray & r, const objlist_naive & world, const skybox_base & skybox, int depth)
{
    if(depth <= 0)
    {
        //cerr << "Ray trace stack full" << endl ;
        return color(0, 0, 0);
    }

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
            double u, v;
            u = i * 1.0 / (constants::image_width - 1);
            v = j * 1.0 / (constants::image_height - 1);
            ray r = cam.get_ray(u, v);
            //cout << "Origin:" << r.orig << " Direction:" << r.dir << endl ;

            color ray_color = ray_trace(r, world, sky, 50);
            pic.push_back(ray_color);
        }
    }

    ofstream fout;
    fout.open("result.ppm");
    ImageOutputPPM()(constants::image_width, constants::image_height, fout, pic);
    fout.close();

    ray r;

    return 0;
}
