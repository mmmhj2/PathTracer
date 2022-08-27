#ifndef VEC_TEMPLATE_H_INCLUDED
#define VEC_TEMPLATE_H_INCLUDED

#include <ostream>
#include <algorithm>
#include <cmath>

template <
    typename T, size_t sz,
    std::enable_if_t<std::is_arithmetic<T>::value, bool> = true
    >
class vec_t
{
private:
    void vec_t_init(size_t cnt)
    {
    }

    template <typename ... Targs>
    void vec_t_init(size_t cnt, const T & val, Targs && ... args)
    {
        d[cnt] = val;
        vec_t_init(cnt + 1, std::forward<Targs>(args)...);
    }

public:
    T d[sz];

    // Constructors
    vec_t()
    {
        std::fill(d, d + sz, 0);
    }

    vec_t(T data[sz])
    {
        std::copy(data, data + sz, d);
    }

    // Defaulting copy and move constructor
    vec_t(vec_t &) = default;
    vec_t(const vec_t &) = default;
    vec_t(vec_t &&) = default;
    vec_t & operator=(vec_t &) = default;
    vec_t & operator=(const vec_t &) = default;
    vec_t & operator=(vec_t &&) = default;

    template <typename ... Targs>
    vec_t(Targs && ... args)
    {
        static_assert(sz == sizeof...(Targs), "Wrong number of arguments");
        vec_t_init(0, std::forward<Targs>(args)...);
    }

    // Operators
    T & operator [](size_t index)
    {
        return d[index];
    }
    T operator [](size_t index) const
    {
        return d[index];
    }

    vec_t operator -() const
    {
        vec_t ret;
        for(size_t i = 0; i < sz; ++i)
            ret[i] = -d[i];
        return ret;
    }

    // Vector operations
    vec_t & operator += (const vec_t & rhs)
    {
        for(size_t i = 0; i < sz; ++i)
            d[i] += rhs[i];
        return *this;
    }
    vec_t & operator -= (const vec_t & rhs)
    {
        for(size_t i = 0; i < sz; ++i)
            d[i] -= rhs[i];
        return *this;
    }

    // Scalar operations
    vec_t & operator *= (const T & val)
    {
        for(size_t i = 0; i < sz; ++i)
            d[i] *= val;
        return *this;
    }
    vec_t & operator /= (const T & val)
    {
        double inverse = 1 / val;
        for(size_t i = 0; i < sz; ++i)
            d[i] *= inverse;
        return *this;
    }

    // Euclidean operations
    T norm_squared() const
    {
        static_assert(std::is_floating_point<T>::value, "No Euclidean norm for this type");
        T result = 0;
        for(size_t i = 0; i < sz; ++i)
            result += d[i] * d[i];
        return result;
    }

    T norm() const
    {
        static_assert(std::is_floating_point<T>::value, "No Euclidean norm for this type");
        return sqrt(this->norm_squared());
    }

    vec_t unit() const
    {
        static_assert(std::is_floating_point<T>::value, "No Euclidean norm for this type");
        return (*this) / norm_squared();
    }

};

template <typename T, size_t sz>
inline std::ostream & operator << (std::ostream & out, const vec_t<T, sz> & v)
{
    out << "(" ;
    for(size_t i = 0; i < sz; i++)
        out << v[i] << (i == sz - 1 ? ')' : ',');
    return out;
}

template <typename T, size_t sz>
inline vec_t<T, sz> operator + (const vec_t<T, sz> & lhs, const vec_t<T, sz> & rhs)
{
    vec_t <T, sz> ret;
    for(size_t i = 0; i < sz; i++)
        ret[i] = lhs[i] + rhs[i];
    return ret;
}

template <typename T, size_t sz>
inline vec_t<T, sz> operator - (const vec_t<T, sz> & lhs, const vec_t<T, sz> & rhs)
{
    vec_t <T, sz> ret;
    for(size_t i = 0; i < sz; i++)
        ret[i] = lhs[i] - rhs[i];
    return ret;
}

// Scalar operations
// NOTE : rhs will be converted to T before multiplying, causing unexpected result if T is integral
// TODO : introducing std::common_type to mitigate the problem
template <typename T, size_t sz>
inline vec_t<T, sz> operator * (const vec_t<T, sz> & lhs, const T & rhs)
{
    vec_t <T, sz> ret;
    for(size_t i = 0; i < sz; i++)
        ret[i] = lhs[i] * rhs;
    return ret;
}

template <typename T, size_t sz>
inline vec_t<T, sz> operator * (const T & lhs, const vec_t<T, sz> & rhs)
{
    return rhs * lhs;
}

template <typename T, size_t sz>
inline vec_t<T, sz> operator / (const vec_t<T, sz> & lhs, const T & rhs)
{
    T inverse = 1 / rhs;
    vec_t <T, sz> ret;
    for(size_t i = 0; i < sz; i++)
        ret[i] = lhs[i] * inverse;
    return ret;
}
template <typename T, size_t sz, typename U>
inline vec_t<T, sz> operator * (const vec_t<T, sz> & lhs, const U & rhs)
{

    vec_t <T, sz> ret;
    for(size_t i = 0; i < sz; i++)
        ret[i] = lhs[i] * rhs;
    return ret;
}

template <typename T, size_t sz, typename U>
inline vec_t<T, sz> operator * (const U & lhs, const vec_t<T, sz> & rhs)
{
    return rhs * lhs;
}

template <typename T, size_t sz, typename U>
inline vec_t<T, sz> operator / (const vec_t<T, sz> & lhs, const U & rhs)
{
    U inverse = 1 / rhs;
    vec_t <T, sz> ret;
    for(size_t i = 0; i < sz; i++)
        ret[i] = lhs[i] * inverse;
    return ret;
}

// Vector operations
// Interior product
template <typename T, size_t sz>
inline vec_t<T, sz> operator * (const vec_t<T, sz> & lhs, const vec_t<T, sz> & rhs)
{
    vec_t <T, sz> ret;
    for(size_t i = 0; i < sz; i++)
        ret[i] = lhs[i] * rhs[i];
    return ret;
}

// Exterior product
template <typename T>
inline vec_t<T, 3> operator ^ (const vec_t<T, 3> & lhs, const vec_t<T, 3> & rhs)
{
    return vec_t<T, 3>(lhs[1] * rhs[2] - lhs[2] * rhs[1],
                       lhs[2] * rhs[0] - lhs[0] * rhs[2],
                       lhs[0] * rhs[1] - lhs[1] * rhs[0]);
}


#endif // VEC_TEMPLATE_H_INCLUDED
