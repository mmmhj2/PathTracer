#include <fstream>
#include "utils/vec3d.h"
#include "image/ImageOutputPPM.h"

using namespace std;

int main()
{
    std::vector <color> pic;

    for(int j = 255; j >= 0; j--)
    {
        for(int i = 255; i >= 0; i--)
        {
            pic.emplace_back(i * 1.0 / 255, j * 1.0 / 255, 0.25);
        }
    }

    ofstream fout;
    fout.open("result.ppm");
    ImageOutputPPM ppm;
    ppm.WriteImage(256, 256, fout, pic);
    fout.close();

    return 0;
}
