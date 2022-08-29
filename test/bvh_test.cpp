#include "utils/defs.h"

#include "objects/sphere.h"
#include "objects/spatial_structure/bvh/bvh.h"
#include "material/lambertian_simple.h"

int main()
{
    auto mat = std::make_shared<lambertian>(color(1, 1, 1));
    auto sp1 = std::make_shared<sphere>(point3(0, 0, 0), 1, mat);

    bvh_tree tree;
    tree.add_object(sp1);
    tree.build();
}
