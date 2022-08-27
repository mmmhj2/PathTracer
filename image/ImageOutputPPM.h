#ifndef IMAGEOUTPUTPPM_H_INCLUDED
#define IMAGEOUTPUTPPM_H_INCLUDED

#include "ImageOutputBase.h"

class ImageOutputPPM : public ImageOutputBase
{
public:
    int WriteImage(size_t width, size_t height, std::ostream & output, const std::vector<color> & data) override
    {
        output << "P3\n";
        output << width << " " << height << "\n";
        output << "255" << "\n";

        for(const auto & pixel : data)
        {
            auto p = color_to_RGB(pixel);
            output << std::get<0>(p) << " " << std::get<1>(p) << " " << std::get<2>(p) << "\n";
        }
        return 0;
    }
};

#endif // IMAGEOUTPUTPPM_H_INCLUDED
