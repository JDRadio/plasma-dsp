////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the AmateurRadio.Engineer project
//! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
//! \date 2020
//! \copyright AmateurRadio.Engineer
////////////////////////////////////////////////////////////////////////////////
template <typename T>
T MATH<T>::sinc(T x)
{
    if (std::abs(x) < 0.001) {
        return std::cos(pi * x / 2.) * std::cos(pi * x / 4.) * std::cos(pi * x / 6.);
    }

    return std::sin(pi * x) / (pi * x);
}
