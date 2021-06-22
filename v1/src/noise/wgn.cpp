////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#include "plasma/noise/wgn.hpp"

namespace plasma::noise {

std::vector<float> wgn::create_single(double power, unsigned int num_samples) noexcept
{
    return create<float>(power, num_samples);
}

std::vector<double> wgn::create_double(double power, unsigned int num_samples) noexcept
{
    return create<double>(power, num_samples);
}

} // namespace
