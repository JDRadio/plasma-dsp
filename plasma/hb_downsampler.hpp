////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the AmateurRadio.Engineer project
//! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
//! \date 2020
//! \copyright AmateurRadio.Engineer
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_HB_DOWNSAMPLER_HPP
#define PLASMA_HB_DOWNSAMPLER_HPP

#include <vector>
#include <complex>

namespace plasma {

template <typename T, typename TC>
class HB_DOWNSAMPLER {
public:
    HB_DOWNSAMPLER(void);
    ~HB_DOWNSAMPLER(void) = default;

    //! Set the filter length
    void set_length(std::size_t length);

    //! Reset the filter state
    void reset(void);

    //! Execute the filter on multiple samples
    void execute(const std::vector<TC>& in, std::vector<TC>& out);

private:
};

#include "detail/hb_downsampler.ipp"

using hb_downsampler_f = HB_DOWNSAMPLER<float, float>;
using hb_downsampler_d = HB_DOWNSAMPLER<double, double>;
using hb_downsampler_cf = HB_DOWNSAMPLER<float, std::complex<float>>;
using hb_downsampler_cd = HB_DOWNSAMPLER<double, std::complex<double>>;

} // namespace

#endif
