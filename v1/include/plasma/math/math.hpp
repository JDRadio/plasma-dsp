////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_DSP_MATH_MATH_HPP_
#define PLASMA_DSP_MATH_MATH_HPP_

#include <cmath>

namespace plasma {

class math
{
    math(void) noexcept = delete;
    ~math(void) noexcept = delete;

public:
    static constexpr double PI = std::acos(-1.0);
    static constexpr double TWO_PI = 2 * PI;
    static constexpr double SQRT2 = std::sqrt(2.0);

    static constexpr double sinc(double x)
    {
        if (x == 0) {
            return 1;
        }

        return std::sin(PI * x) / (PI * x);
    }
};

} // namespace

#endif
