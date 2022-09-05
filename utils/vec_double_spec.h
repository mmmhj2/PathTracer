#ifndef VEC_DOUBLE_SPEC_H_INCLUDED
#define VEC_DOUBLE_SPEC_H_INCLUDED

#include <valarray>

template <>
class vec_t <double, 3, true>
{
    double d[3];
    static constexpr size_t sz = 3;
public:
    vec_t()
    {
        d[0] = d[1] = d[2] = 0;
    }

    vec_t(const vec_t &) = default;
    vec_t(vec_t &&) = default;
    vec_t & operator = (const vec_t &) = default;
    vec_t & operator = (vec_t &&) = default;

    vec_t(double x, double y, double z)
    {
        d[0] = x, d[1] = y, d[2] = z;
    }

    // Operators
    double & operator [](size_t index)
    {
        return d[index];
    }
    const double & operator [](size_t index) const
    {
        return d[index];
    }

    vec_t operator -() const
    {
        vec_t ret;
        ret.d[0] = -d[0];
        ret.d[1] = -d[1];
        ret.d[2] = -d[2];
        return ret;
    }

    // Vector operations
    vec_t & operator += (const vec_t & rhs)
    {
        d[0] += rhs.d[0];
        d[1] += rhs.d[1];
        d[2] += rhs.d[2];
        return *this;
    }
    vec_t & operator -= (const vec_t & rhs)
    {
        d[0] -= rhs.d[0];
        d[1] -= rhs.d[1];
        d[2] -= rhs.d[2];
        return *this;
    }

    // Scalar operations
    vec_t & operator *= (const double & val)
    {
        d[0] *= val;
        d[1] *= val;
        d[2] *= val;
        return *this;
    }
    vec_t & operator /= (const double & val)
    {
        double inverse = 1 / val;
        d[0] *= inverse;
        d[1] *= inverse;
        d[2] *= inverse;
        return *this;
    }

    // Euclidean operations
    double norm_squared() const
    {
        return d[0] * d[0] + d[1] * d[1] + d[2] * d[2];
    }

    double norm() const
    {
        return std::sqrt(this->norm_squared());
    }

    vec_t unit() const
    {
        return (*this) / norm();
    }

    friend vec_t<double, 3> operator + (const vec_t<double, 3> & lhs, const vec_t<double, 3> & rhs)
    {
        vec_t <double, 3> ret;
        ret.d[0] = lhs.d[0] + rhs.d[0];
        ret.d[1] = lhs.d[1] + rhs.d[1];
        ret.d[2] = lhs.d[2] + rhs.d[2];
        return ret;
    }

    friend vec_t<double, 3> operator - (const vec_t<double, 3> & lhs, const vec_t<double, 3> & rhs)
    {
        vec_t <double, 3> ret;
        ret.d[0] = lhs.d[0] - rhs.d[0];
        ret.d[1] = lhs.d[1] - rhs.d[1];
        ret.d[2] = lhs.d[2] - rhs.d[2];
        return ret;
    }

    friend vec_t<double, 3> operator * (const vec_t<double, 3> & lhs, double rhs)
    {
        vec_t <double, 3> ret;
        ret.d[0] = lhs.d[0] * rhs;
        ret.d[1] = lhs.d[1] * rhs;
        ret.d[2] = lhs.d[2] * rhs;
        return ret;
    }

    friend vec_t<double, 3> operator * (double lhs, const vec_t<double, 3> & rhs)
    {
        return rhs * lhs;
    }

    friend double operator * (const vec_t<double, 3> & lhs, const vec_t<double, 3> & rhs)
    {
        return lhs.d[0] * rhs.d[0] + lhs.d[1] * rhs.d[1] + lhs.d[2] * rhs.d[2];
    }
};


#endif // VEC_DOUBLE_SPEC_H_INCLUDED
