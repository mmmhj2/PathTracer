#include <fstream>
#include <iostream>
#include "utils/defs.h"
#include "image/ImageOutputPPM.h"
#include "ray/skybox_base.h"
#include "objects/sphere.h"
#include "objects/objlist_naive.h"
#include "ray/camera.h"
#include "utils/concurrent_trace.h"
#include <future>
using namespace std;

int main()
{
    camera cam;
    skybox_base sky;
    objlist_naive world;
    world.push_back(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.push_back(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

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
    }

    std::vector <std::future<std::vector<color>>> async_task;
    for(int i = 0; i < constants::blocks; i++)
        async_task.push_back(std::async(concurrent::trace_block, infos[i]));

    std::vector <color> pic;
    for(int i = 0; i < constants::blocks; i++)
    {
        std::vector <color> block = async_task[i].get();
        pic.insert(pic.end(), block.cbegin(), block.cend());
    }


    ofstream fout;
    fout.open("result.ppm");
    ImageOutputPPM()(constants::image_width, constants::image_height, fout, pic);
    fout.close();

    ray r;

    return 0;
}
