////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#include "plasma/filters/windowed.hpp"
#include "plasma/math/math.hpp"

namespace plasma::filters {

std::vector<float> windowed::create_taps_single(double fc, double df, double att, windows::window const& win) noexcept
{
    return create_taps<float>(fc, df, att, win);
}

std::vector<double> windowed::create_taps_double(double fc, double df, double att, windows::window const& win) noexcept
{
    return create_taps<double>(fc, df, att, win);
}

} // namespace
