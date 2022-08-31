#include <iostream>
#define BLINN_PHONG_DEBUG
#include "material/blinn_phong.h"

using namespace std;

int main()
{

    auto mat = std::make_shared<blinn_phong_naive> (color(0, 0, 0), color(0, 0, 0), 1);

    ray test_ray;
    hit_record test_hit;
    color result;
    ray scattered;

    test_ray.orig = vec3(0, 1, -1);
    test_ray.dir = vec3(0, -1, 1);
    test_hit.p = point3(0, 0, 0);
    test_hit.normal = vec3(0, 1, 0);

    for(int i = 0 ; i < 50; i++)
        mat->evaluateScatter(test_ray, test_hit, result, scattered);
}
