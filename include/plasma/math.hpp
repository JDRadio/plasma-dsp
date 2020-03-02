#pragma once

#include <complex>
#include <cmath>

using namespace std;

namespace plasma {

class math {
public:
    math(void) = delete;
    ~math(void) = delete;

    static constexpr double pi = acos(-1.0);
    static constexpr double two_pi = 2.0 * pi;
    static constexpr double sqrt2 = sqrt(2.0);
    static constexpr double sqrt1_2 = 1.0 / sqrt2;

    // normalized sinc: sin(\pi x) / (\pi x)
    static double sinc(double x);
};

} // namespace
