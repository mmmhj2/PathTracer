#ifndef PDF_BASE_H_INCLUDED
#define PDF_BASE_H_INCLUDED

// Base class for probability density function
class pdf_base
{
public:

    virtual double value(const vec3 & direction) const = 0;
    virtual vec3 sample_hemisphere() const = 0;
};

#endif // PDF_BASE_H_INCLUDED
