#include "plasma/dsp/math.hpp"

namespace dsp {

// normalized sinc: sin(\pi x) / (\pi x)
double math::sinc(double x)
{
    if (abs(x) < 0.01) {
        return cos(pi * x / 2.) * cos(pi * x / 4.) * cos(pi * x / 6.);
    }

    return sin(pi * x) / (pi * x);
}

// normalized sinc: sin(\pi x) / (\pi x)
float math::sincf(float x)
{
    if (abs(x) < 0.01f) {
        return cos(pif * x / 2.f) * cos(pif * x / 4.f) * cos(pif * x / 6.f);
    }

    return sin(pif * x) / (pif * x);
}

} // namespace
