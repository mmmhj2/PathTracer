#ifndef CORNELLBOX_DEF_H_INCLUDED
#define CORNELLBOX_DEF_H_INCLUDED

namespace cornell_box
{
    vec3 vert_right[] =
    {
        vec3(0, 0, 555),
        vec3(0, 0, 0),
        vec3(0, 555, 0),
        vec3(0, 555, 555),
        vec3(0, 0, 555)
    };
    vec3 vert_left[] =
    {
        vec3(555, 0, 555),
        vec3(555, 0, 0),
        vec3(555, 555, 0),
        vec3(555, 555, 555),
        vec3(555, 0, 555)
    };
    vec3 vert_back[] =
    {
        vec3(0, 0, 0),
        vec3(555, 0, 0),
        vec3(555, 555, 0),
        vec3(0, 555, 0),
        vec3(0, 0, 0)
    };
    vec3 vert_down[] =
    {
        vec3(0, 0, 555),
        vec3(555, 0, 555),
        vec3(555, 0, 0),
        vec3(0, 0, 0),
        vec3(0, 0, 555)
    };
    vec3 vert_up[] =
    {
        vec3(0, 555, 0),
        vec3(555, 555, 0),
        vec3(555, 555, 555),
        vec3(0, 555, 555),
        vec3(0, 555, 0)
    };

    vec3 vert_up_light[] =
    {
        vec3(213, 554, 227),
        vec3(343, 554, 227),
        vec3(343, 554, 332),
        vec3(213, 554, 332),
        vec3(213, 554, 227)
    };

    std::pair <double, double> uv[5] =
    {
        std::make_pair(0.0, 0.0),
        std::make_pair(1.0, 0.0),
        std::make_pair(1.0, 1.0),
        std::make_pair(0.0, 1.0),
        std::make_pair(0.0, 0.0)
    };
};

#endif // CORNELLBOX_DEF_H_INCLUDED
