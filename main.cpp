#include <fstream>
#include <iostream>
#include "utils/defs.h"
#include "image/ImageOutputPPM.h"
#include "ray/ray.h"
#include "ray/skybox_base.h"
using namespace std;

int main()
{
    skybox_base sky;
    std::vector <color> pic;

    for(int j = constants::image_height - 1; j >= 0; j--)
    {
        for(int i = constants::image_width - 1; i >= 0; i--)
        {
            double u, v;
            u = i * 1.0 / (constants::image_width - 1);
            v = j * 1.0 / (constants::image_height - 1);
            ray r(constants::origin,
                  constants::lower_left +
                  u * constants::horizontal +
                  v * constants::vertical -
                  constants::origin);
            //cout << "Origin:" << r.orig << " Direction:" << r.dir << endl ;
            pic.push_back(sky(r));
        }
    }

    ofstream fout;
    fout.open("result.ppm");
    ImageOutputPPM()(constants::image_width, constants::image_height, fout, pic);
    fout.close();

    ray r;

    return 0;
}
