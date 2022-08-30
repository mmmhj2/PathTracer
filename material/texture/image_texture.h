#ifndef IMAGE_TEXTURE_H_INCLUDED
#define IMAGE_TEXTURE_H_INCLUDED

#include "texture_base.h"
#include "external/stb.h"

class image_texture : public uv_texture
{
private:
    constexpr static int bytes_per_pixel = 3;
    constexpr static double color_scale = 1.0 / 255.0;

    unsigned char * data;
    int width, height;
    int bytes_per_line;

public:

    image_texture() = delete;
    image_texture(const image_texture &) = delete;
    operator = (const image_texture &) = delete;

    image_texture(const char * filename)
    {
        int channels_in_file = bytes_per_pixel;
        data = stbi_load(filename, &width, &height, &channels_in_file, bytes_per_pixel);

        if(data == nullptr)
        {
            width = height = 0;
            throw std::runtime_error(stbi_failure_reason());
        }


        bytes_per_line = bytes_per_pixel * width;
    }

    virtual ~image_texture()
    {
        if(data != nullptr)
            stbi_image_free(data);
    }

    virtual color get_color(double u, double v, [[maybe_unused]] const vec3 & p) const override
    {
        if(data == nullptr)
            return color(0, 1, 1);

        u = tools::clamp(u, 0.0, 1.0);
        v = 1.0 - tools::clamp(v, 0.0, 1.0);

        int i, j;
        i = std::min(static_cast<int>(u * width), width - 1);
        j = std::min(static_cast<int>(v * height), height - 1);
        auto pixel = data + j * bytes_per_line + i * bytes_per_pixel;
        //std::cerr << "Reading from " << i << " " << j << std::endl ;
        return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
    }

};

#endif // IMAGE_TEXTURE_H_INCLUDED
