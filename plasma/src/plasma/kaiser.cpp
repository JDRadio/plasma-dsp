////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#include "kaiser.hpp"
#include <cmath>

namespace plasma {

//! Pi
static constexpr auto PI = std::acos(-1.0);

unsigned int kaiser::get_order(double att, double df) noexcept
{
    unsigned int n;

    if (att >= 21.0) {
        n = std::ceil((att - 7.95) / (2.285 * (2.0 * PI * df)));
    }
    else {
        n = std::ceil(5.79 / (2.0 * PI * df));
    }

    if (n % 2 == 0) {
        return n + 1;
    }

    return n;
}

double kaiser::calculate_beta(double att) noexcept
{
    att = std::abs(att);

    if (att > 50.0) {
        return 0.1102 * (att - 8.7);
    }
    else if (att < 21.0) {
        return 0.0;
    }
    else {
        return 0.5842 * std::pow(att - 21.0, 0.4) + 0.07886 * (att - 21.0);
    }
}

double kaiser::window(double beta, int n, int big_n) noexcept
{
    double t = static_cast<double>(n) - static_cast<double>(big_n - 1) / 2.0;
    double r = 2.0 * t / static_cast<double>(big_n - 1);
    double a = std::cyl_bessel_i(0, beta * std::sqrt(1.0 - r * r));
    double b = std::cyl_bessel_i(0, beta);

    return a / b;
}

std::vector<float> kaiser::create_taps(double fc, double df, double att) noexcept
{
    double beta = calculate_beta(att);
    unsigned int len = get_order(att, df);
    std::vector<float> taps(len);

    for (unsigned int n = 0; n < len; ++n) {
        double t = n - (len - 1) / 2.0;
        taps[n] = 2.0 * fc * sinc(2.0 * fc * t) * window(beta, n, len);
    }

    return taps;
}

double kaiser::sinc(double x) noexcept
{
    if (x == 0) {
        return 1;
    }

    return std::sin(PI * x) / (PI * x);
}

} // namespace
