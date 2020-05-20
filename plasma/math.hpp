////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the AmateurRadio.Engineer project
//! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
//! \date 2020
//! \copyright AmateurRadio.Engineer
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_MATH_HPP
#define PLASMA_MATH_HPP

#include <complex>
#include <cmath>

namespace plasma {

template <typename T>
class MATH {
public:
    MATH(void) = delete;
    ~MATH(void) = delete;

    static constexpr T pi = std::acos(-1.0);
    static constexpr T two_pi = 2.0 * pi;
    static constexpr T sqrt2 = std::sqrt(2.0);
    static constexpr T sqrt1_2 = 1.0 / sqrt2;

    //! normalized sinc: sin(\pi x) / (\pi x)
    static T sinc(T x);
};

#include "detail/math.ipp"

using math_f = MATH<float>;
using math_d = MATH<double>;

} // namespace

#endif
