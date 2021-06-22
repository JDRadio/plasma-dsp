////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_DSP_WINDOWS_WINDOW_HPP_
#define PLASMA_DSP_WINDOWS_WINDOW_HPP_

namespace plasma::windows {

class window
{
public:
    //! Calculate the window coefficient
    //! \param[in] n Current sample number
    //! \param[in] big_n Total sample count
    //! \return Window coefficient
    virtual double get(int n, int big_n) const noexcept = 0;

    //! Calculate the minimum filter order
    //! \param[in] att Stop-band attenuation
    //! \param[in] df Transition bandwidth
    //! \return Minimum filter order
    virtual unsigned int get_order(double att, double df) const noexcept = 0;
};

} // namespace

#endif
