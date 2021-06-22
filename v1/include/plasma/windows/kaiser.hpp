////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_DSP_WINDOWS_KAISER_HPP_
#define PLASMA_DSP_WINDOWS_KAISER_HPP_

#include "window.hpp"

namespace plasma::windows {

class kaiser : public window
{
public:
    //! Create a Kaiser window using the beta parameter
    //! \param[in] beta Kaiser beta
    kaiser(double beta) noexcept;

    //! \copydoc window::get(int, int)
    double get(int n, int big_n) const noexcept override;

    //! Calculate the minimum Kaiser windowed filter order
    //! \param[in] att Stop-band attenuation
    //! \param[in] df Transition bandwidth
    //! \return Minimum Kaiser filter order
    unsigned int get_order(double att, double df) const noexcept override;

    //! Calculate the Kaiser beta
    //! \param[in] att Stop-band attenuation
    //! \return Kaiser beta
    static double get_beta(double att) noexcept;

private:
    //! Beta
    double beta_;
};

} // namespace

#endif
