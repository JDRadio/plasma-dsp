////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_DSP_KAISER_HPP_
#define PLASMA_DSP_KAISER_HPP_

#include <vector>

namespace plasma {

//! Kaiser filter generator
class kaiser
{
    kaiser(void) noexcept = delete;
    ~kaiser(void) noexcept = delete;

public:
    //! Normalized sinc
    //! \param[in] x x
    //! \return Normalized sinc of x
    static double sinc(double x) noexcept;

    //! Creates a Kaiser-windowed filter
    //! \param[in] fc Cut-off frequency
    //! \param[in] df Transition bandwidth
    //! \param[in] att Stop-band attenuation
    //! \return Filter taps
    static std::vector<float> create_taps(double fc, double df, double att) noexcept;

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

#endif
