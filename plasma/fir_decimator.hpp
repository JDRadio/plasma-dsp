////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the AmateurRadio.Engineer project
//! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
//! \date 2020
//! \copyright AmateurRadio.Engineer
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_FIR_DECIMATOR_HPP
#define PLASMA_FIR_DECIMATOR_HPP

#include "fir.hpp"
#include <vector>
#include <complex>

namespace plasma {

template <typename T, typename T_TAPS = double>
class FIR_DECIMATOR {
public:
    FIR_DECIMATOR(void);
    ~FIR_DECIMATOR(void) = default;

    void set_taps(std::vector<T_TAPS> const& taps);

    void set_factor(unsigned int factor);
    unsigned int get_factor(void) const;

    void reset(void);

    std::vector<T> execute(std::vector<T> const& in);

private:
    unsigned int factor_;
    unsigned int samples_;
    FIR<T,T_TAPS> filter_;
};

#include "detail/fir_decimator.ipp"

using fir_decimator_ff = FIR_DECIMATOR<float, float>;
using fir_decimator_dd = FIR_DECIMATOR<double, double>;
using fir_decimator_cff = FIR_DECIMATOR<std::complex<float>, float>;
using fir_decimator_cdd = FIR_DECIMATOR<std::complex<double>, double>;

using fir_decimator_cfcf = FIR_DECIMATOR<std::complex<float>, std::complex<float>>;
using fir_decimator_cdcd = FIR_DECIMATOR<std::complex<double>, std::complex<double>>;

} // namespace

#endif
