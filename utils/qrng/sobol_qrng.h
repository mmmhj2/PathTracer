#ifndef SOBOL_QRNG_H_INCLUDED
#define SOBOL_QRNG_H_INCLUDED

#include <algorithm>
#include "external/sobol.hpp"

namespace sobol_generator
{

constexpr int sequence_dim = 2;
constexpr size_t sequence_max = 8192;
std::unique_ptr <double[]> sequence;

void init_generator()
{
    sequence.reset(i8_sobol_generate(sequence_dim, sequence_max, 0));
    std::random_shuffle(&sequence[0], &sequence[sequence_dim * sequence_max]);
}

std::pair <double, double> random_double_pair()
{
    thread_local static size_t index = 0;
    auto ret = std::make_pair(sequence[index * 2], sequence[index * 2 + 1]);
    index = (index + 1) % sequence_max ;
    return ret;
}

}

#endif // SOBOL_QRNG_H_INCLUDED
