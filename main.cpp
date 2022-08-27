#include <fstream>
#include <iostream>
#include "utils/defs.h"
#include "image/ImageOutputPPM.h"
#include "ray/skybox_base.h"
#include "objects/sphere.h"
#include "objects/objlist_naive.h"
using namespace std;

int main()
{
    skybox_base sky;
    std::vector <color> pic;

    objlist_naive world;
    world.push_back(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.push_back(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

    cout << "Lower left : " << constants::lower_left << endl ;
    cout << "Horizontal : " << constants::horizontal << endl ;
    cout << "Vertical : " << constants::vertical << endl ;

    for(int j = constants::image_height - 1; j >= 0; j--)
    {
        for(int i = 0; i < constants::image_width; i++)
        {
            double u, v;
            u = i * 1.0 / (constants::image_width - 1);
            v = j * 1.0 / (constants::image_height - 1);
            ray r(constants::origin,
                  constants::lower_left +
                  u * constants::horizontal +
                  v * constants::vertical
                  );
            //cout << "Origin:" << r.orig << " Direction:" << r.dir << endl ;

            color ray_color;
            hit_record rec;
            if(world.hit(r, 0, constants::dinf, rec))
            {
                pic.push_back(0.5 * (rec.normal + color(1,1,1)));
            }
            else
            {
                pic.push_back(sky(r));
            }

        }
    }

    ofstream fout;
    fout.open("result.ppm");
    ImageOutputPPM()(constants::image_width, constants::image_height, fout, pic);
    fout.close();

    ray r;

    return 0;
}
