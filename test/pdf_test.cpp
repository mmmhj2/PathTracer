#include "utils/orthonormal_basis.h"
#include "material/pdf/pdf.h"
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

    for(int i = 0; i <= 100; i++)
        cout << triangle->sample(vec3(0, 0, 1)) << endl ;
    return 0;
}
