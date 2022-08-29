#ifndef IMAGEOUTPUTBASE_H_INCLUDED
#define IMAGEOUTPUTBASE_H_INCLUDED

#include <ostream>
#include <vector>
#include "utils/color.h"

class ImageOutputBase
{
public:
    virtual int operator()(size_t width, size_t height, std::ostream & output, const std::vector<color> & data) = 0;
};

#endif // IMAGEOUTPUTBASE_H_INCLUDED
