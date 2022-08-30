#include "utils/defs.h"

#include "objects/sphere.h"
#include "objects/spatial_structure/bvh/bvh.h"
#include "material/lambertian_simple.h"

int main()
{
    auto tex = std::make_shared<solid_color>(color(1, 1, 1));
    auto mat = std::make_shared<lambertian>(tex);
    bvh_tree tree;
    //tree.add_object(std::make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, mat));
    tree.add_object(std::make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, mat));
    tree.add_object(std::make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, mat));
    //tree.add_object(std::make_shared<sphere>(point3(-1.0,    0.0, -1.0), -0.45, mat));
    tree.add_object(std::make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, mat));
    tree.build();

    tree.debug();
}
