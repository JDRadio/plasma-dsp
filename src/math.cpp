#include "plasma/math.hpp"

namespace plasma {

// normalized sinc: sin(\pi x) / (\pi x)
double math::sinc(double x)
{
    if (abs(x) < 0.01) {
        return cos(pi * x / 2.) * cos(pi * x / 4.) * cos(pi * x / 6.);
    }

    return sin(pi * x) / (pi * x);
}

} // namespace
