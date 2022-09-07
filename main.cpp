#include <fstream>
#include <iostream>
#include <future>
#include <bitset>

#include "utils/qrng/sobol_qrng.h"

using namespace std;

int main()
{
    sobol_generator::init_generator();

    for(int i = 0; i < 64; i++)
    {
        auto rand_pair = sobol_generator::random_double_pair();
        cout << rand_pair.first << " " << rand_pair.second << endl ;
    }

    return 0;
}
