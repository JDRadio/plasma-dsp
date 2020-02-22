#pragma once

#include <complex>
#include <cmath>

using namespace std;

namespace dsp {

class math {
public:
    math(void) = delete;
    ~math(void) = delete;

    static constexpr float pif = acos(-1.f);
    static constexpr double pi = acos(-1.);

    // normalized sinc: sin(\pi x) / (\pi x)
    static double sinc(double x);
    static float sincf(float x);
};

} // namespace
