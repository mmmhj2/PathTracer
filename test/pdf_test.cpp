#include "utils/orthonormal_basis.h"
#include <iostream>
using namespace std;

int main()
{
    onb x_plus(vec3(1, 0, 0));
    onb y_plus(vec3(0, 1, 0));
    onb z_plus(vec3(0, 0, 1));

    cout << "x_plus : " << x_plus[0] << " " << x_plus[1] << " " << x_plus[2] << endl ;
    cout << "y_plus : " << y_plus[0] << " " << y_plus[1] << " " << y_plus[2] << endl ;
    cout << "z_plus : " << z_plus[0] << " " << z_plus[1] << " " << z_plus[2] << endl ;
}
