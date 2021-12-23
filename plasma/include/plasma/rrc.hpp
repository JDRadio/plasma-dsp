////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_DSP_RRC_HPP_
#define PLASMA_DSP_RRC_HPP_

#include "math.hpp"
#include <vector>
#include <cmath>

namespace plasma {

//! RRC filter generator
template <typename T>
class rrc
{
    rrc(void) noexcept = delete;
    ~rrc(void) noexcept = delete;

public:
    //! Creates a RRC filter
    //! \param[in] k Samples per symbol
    //! \param[in] m Symbol delay
    //! \param[in] r Roll-off factor
    //! \return Filter taps
    static std::vector<T> create_taps(unsigned int k, unsigned int m, double r) noexcept;
};

} // namespace

#include "rrc.ipp"

#endif
