////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#include "plasma/windows/kaiser.hpp"
#include <cmath>

namespace plasma::windows {

kaiser::kaiser(double beta) noexcept :
    beta_{beta}
{
}

unsigned int kaiser::get_order(double df, double att) const noexcept
{
    unsigned int n;

    if (att >= 21.0) {
        n = std::ceil((att - 7.95) / (2.285 * (2.0 * M_PI * df)));
    }
    else {
        n = std::ceil(5.79 / (2.0 * M_PI * df));
    }

    if (n % 2 == 0) {
        return n + 1;
    }

    return n;
}

double kaiser::get_beta(double att) noexcept
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

double kaiser::get(int n, int big_n) const noexcept
{
    double t = static_cast<double>(n) - static_cast<double>(big_n - 1) / 2.0;
    double r = 2.0 * t / static_cast<double>(big_n - 1);
    double a = std::cyl_bessel_i(0, beta_ * std::sqrt(1.0 - r * r));
    double b = std::cyl_bessel_i(0, beta_);

    return a / b;
}

} // namespace
