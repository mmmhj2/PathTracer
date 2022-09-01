#include <iostream>
#define TRIANGLE_TEST
#include "material/lambertian_simple.h"
#include "material/texture/texture_base.h"
#include "objects/triangle_flat.h"

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
}
