#include <iostream>
#include "utils/vec3d.h"

using namespace std;

int main()
{
    vec3d vec1(1,2,3);
    vec3d vec2(1,2,4);

    cout << vec1 + vec2 << endl ;
    cout << vec1 - vec2 << endl ;
    cout << vec1 * vec2 << endl ;
    cout << (vec1 ^ vec2) << endl ;
    return 0;
}
