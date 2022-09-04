#include "utils/orthonormal_basis.h"
#include "material/bsdf/bsdf.h"
#include "objects/triangle_flat.h"

#include <iostream>
using namespace std;

int main()
{
    vec3 vert[3] =
    {
        vec3(0, 0, 0),
        vec3(1, 0, 0),
        vec3(0, 1, 0)
    };
    std::pair <double, double> uv[3] =
    {
        std::make_pair(0, 0),
        std::make_pair(1, 0),
        std::make_pair(0, 1)
    };

    auto triangle = std::make_shared<triangle_flat>(vert, uv, nullptr);

    hit_record test_rec;
    ray test_ray;
    test_rec.p = point3(0, 0, 0);
    test_rec.normal = vec3(0, 0, 1);
    auto lamBSDF = std::make_shared<BSDF_lambertian>(test_rec, nullptr);

    for(int i = 0; i < 50; i++)
    {
        BSDF_Sample spl;
        lamBSDF->sample(test_ray, spl);
        cout << spl.in_rev.origin() << " " << spl.in_rev.direction() << " " << spl.pdf << endl ;
    }

    return 0;
}
