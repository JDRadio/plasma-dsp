////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_DSP_FILTERS_KAISER_HPP_
#define PLASMA_DSP_FILTERS_KAISER_HPP_

#include "plasma/windows/window.hpp"
#include "plasma/math/math.hpp"
#include <vector>

namespace plasma::filters {

class windowed
{
    windowed(void) noexcept = delete;
    ~windowed(void) noexcept = delete;

public:
    //! Creates a windowed filter with single-precision taps
    //! \param[in] fc Cut-off frequency
    //! \param[in] df Transition bandwidth
    //! \param[in] att Stop-band attenuation
    //! \return Filter taps
    static std::vector<float> create_taps_single(double fc, double df, double att, windows::window const& win) noexcept;

    //! Creates a windowed filter with double-precision taps
    //! \param[in] fc Cut-off frequency
    //! \param[in] df Transition bandwidth
    //! \param[in] att Stop-band attenuation
    //! \return Filter taps
    static std::vector<double> create_taps_double(double fc, double df, double att, windows::window const& win) noexcept;

    template <typename T>
    static std::vector<T> create_taps(double fc, double df, double att, windows::window const& win) noexcept
    {
        unsigned int const len = win.get_order(df, att);
        std::vector<T> taps(len);

        for (unsigned int n = 0; n < len; ++n) {
            double t = n - (len - 1) / 2.0;
            taps[n] = 2.0 * fc * math::sinc(2.0 * fc * t) * win.get(n, len);
        }

        return taps;
    }
};

} // namespace

#endif
