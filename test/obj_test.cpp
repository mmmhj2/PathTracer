#include "ray/light/light.h"
#include "objects/triangle_flat.h"

#include "material/lambertian_simple.h"
#include "material/emissive_simple.h"
#include "material/dielectric.h"
#include "material/texture/texture_base.h"
#include "material/texture/checker_texture.h"
#include "material/texture/image_texture.h"
#include "objects/triangle_flat.h"
#include "objects/sphere.h"
#include "objects/spatial_structure/bvh/bvh.h"
#include "ray/camera.h"
#include "skybox/skybox_colored.h"
#include "tracer/tracer.h"
#include "image/ImageOutputPPM.h"

#include "cornellbox_def.h"
#include "external/tol.h"

#include <iostream>
#include <fstream>
#include <future>
#include <list>

using namespace std;

int main()
{
    using namespace cornell_box;
    sobol_generator::init_generator();
    camera cam(point3(0,0,15), point3(0,0,0), vec3(0,1,0), constants::pi / 4.5, 1.0);
    skybox_color_gradient sky;
    auto world = make_shared<bvh_tree>();
    auto red_tex = make_shared<solid_color>(color(.65, .05, .05));
    auto red_mat = make_shared<lambertian>(red_tex);

    // Load obj
    tinyobj::attrib_t attrib;
    std::vector <tinyobj::shape_t> shapes;
    std::vector <tinyobj::material_t> mats;
    std::string error;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &mats, &error, "teapot.obj");
    if(!ret)
    {
        cout << "Cannot load obj file" << endl ;
        return -1;
    }

    std::list <vec3> vecs;
    std::list <vec3> normals;
    std::list <std::pair<double, double>> uvs;

    for(size_t s = 0; s < shapes.size(); s++)
    {
        // Loop over faces(polygon)
        cout << "Shape no. " << s << endl ;
        size_t index_offset = 0;
        for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
        {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
//            assert(fv == 3);

            // Loop over vertices in the face.

            triangle_vert vert;
            triangle_uv uv;

            for(size_t v = 0; v < fv; v++)
            {
                double vx, vy, vz;
                double tu, tv;
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
                //cout << vx << "," << vy << "," << vz << endl ;
                vecs.emplace_back(vx, vy, vz);

                if(idx.texcoord_index >= 0)
                {
                    tu = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    tv = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                }
                else
                {
                    cout << "Does not found UV coordinate" << endl ;
                    switch(v)
                    {
                    case 0:
                        tu = 0, tv = 0;
                        break;
                    case 1:
                        tu = 1, tv = 0;
                        break;
                    case 2:
                        tu = 0, tv = 1;
                    }
                }
                uvs.emplace_back(tu, tv);

                vert[v] = &(*vecs.rbegin());
                uv[v] = &(*uvs.rbegin());
            }

            world->add_object(std::make_shared<triangle_flat>(vert, uv, red_mat));
            index_offset += fv;
        }
    }
    cout << "Building BVH tree" << endl ;
    world->build();
    std::vector <concurrent::block_info> infos(constants::blocks);
    int image_width = 600;
    int image_height = 600;
    concurrent::fill_info(infos, image_width, image_height, 1, constants::blocks, &cam, world.get(), &sky, nullptr);

    std::vector <std::future<std::vector<color>>> async_task;
    for(int i = 0; i < constants::blocks; i++)
        async_task.push_back(std::async(concurrent::trace_block, std::cref(infos[i])));

    pbar::init_curses();
    while(!pbar::print_progressbar(infos))
    {
        std::this_thread::sleep_for(100ms);
    }
    pbar::decon_curses();

    std::vector <color> pic;
    for(int i = 0; i < constants::blocks; i++)
    {
        std::vector <color> block = async_task[i].get();
        pic.insert(pic.end(), block.cbegin(), block.cend());
    }


    ofstream fout;
    fout.open("obj.ppm");
    ImageOutputPPM()(image_width, image_height, fout, pic);
    fout.close();

}
