////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_DSP_KAISER_HPP_
#define PLASMA_DSP_KAISER_HPP_

#include "math.hpp"
#include <vector>
#include <cmath>

namespace plasma {

//! Kaiser filter generator
template <typename T>
class kaiser
{
    kaiser(void) noexcept = delete;
    ~kaiser(void) noexcept = delete;

public:
    //! Creates a Kaiser-windowed filter
    //! \param[in] fc Cut-off frequency
    //! \param[in] df Transition bandwidth
    //! \param[in] att Stop-band attenuation
    //! \return Filter taps
    static std::vector<T> create_taps(double fc, double df, double att) noexcept;

    //! Calculate the Kaiser window coefficient
    //! \param[in] beta Kaiser beta
    //! \param[in] n Current sample number
    //! \param[in] big_n Total sample count
    //! \return Kaiser window coefficient
    static double window(double beta, int n, int big_n) noexcept;

    //! Calculate the Kaiser beta
    //! \param[in] att Stop-band attenuation
    //! \return Kaiser beta
    static double calculate_beta(double att) noexcept;

    //! Calculate the minimum Kaiser filter order
    //! \param[in] att Stop-band attenuation
    //! \param[in] df Transition bandwidth
    //! \return Minimum Kaiser filter order
    static unsigned int get_order(double att, double df) noexcept;
};

} // namespace

#include "kaiser.ipp"

#endif
