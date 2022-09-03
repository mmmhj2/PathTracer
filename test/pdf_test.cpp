#include "utils/orthonormal_basis.h"
#include "material/pdf/pdf_base.h"
#include <iostream>
using namespace std;

int main()
{
    pdf_cosine pc(vec3(0, 0, 1));
    pdf_uniform pu(vec3(0, 0, 1));

    cout << "Test;" << endl ;
    cout << "::X::Y::Z;" << endl ;
    for(int i = 0; i < 5000; i++)
    {
        vec3 v = pc.sample_hemisphere();
        cout << i << "::" << v[0] << "::" << v[1] << "::" << v[2] << "::0::1::A::1::0::0::0::0;" << endl ;
        v = pu.sample_hemisphere();
        cout << i << "::" << v[0] << "::" << v[1] << "::" << v[2] << "::0::1::B::1::0::0::0::0;" << endl ;
    }

}
