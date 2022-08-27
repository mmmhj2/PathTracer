#ifndef IMAGEOUTPUTBASE_H_INCLUDED
#define IMAGEOUTPUTBASE_H_INCLUDED

#include <ostream>

class ImageOutputBase
{
public:
    virtual WriteImage(size_t width, size_t height, std::ostream & output, void * data) = 0;
};

#endif // IMAGEOUTPUTBASE_H_INCLUDED
