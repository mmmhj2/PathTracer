#include "utils/vec3d.h"
#include <iostream>
using namespace std;

int main()
{
    vec3d vd1 (1, 2, 3);
    vec3d vd2 (4, 5, 6);

    cout << (vd1 + vd2) << endl ;
    cout << (vd1 - vd2) << endl ;
    cout << (vd1 * vd2) << endl ;
    cout << (vd1 ^ vd2) << endl ;
    cout << vd1 * 2.0 << endl ;
    cout << vd2 / 2.0 << endl ;

    vec3i vi1 (1, 2, 3);
    vec3i vi2 (4, 5, 6);

    cout << (vi1 + vi2) << endl ;
    cout << (vi1 - vi2) << endl ;
    cout << (vi1 * vi2) << endl ;
    cout << (vi1 ^ vi2) << endl ;
    cout << vi1 * 2 << endl ;
    cout << vi2 / 2.0 << endl ;
}
