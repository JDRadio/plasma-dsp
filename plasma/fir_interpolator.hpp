////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the AmateurRadio.Engineer project
//! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
//! \date 2020
//! \copyright AmateurRadio.Engineer
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_FIR_INTERPOLATOR_HPP
#define PLASMA_FIR_INTERPOLATOR_HPP

#include "fir.hpp"
#include <vector>
#include <complex>

namespace plasma {

template <typename T, typename T_TAPS = double>
class FIR_INTERPOLATOR {
public:
    FIR_INTERPOLATOR(void);
    ~FIR_INTERPOLATOR(void) = default;

    void set_taps(std::vector<T_TAPS> const& taps);

    void set_factor(unsigned int factor);
    unsigned int get_factor(void) const;

    unsigned int get_delay(void) const;

    void reset(void);

    std::vector<T> execute(std::vector<T> const& in);

private:
    unsigned int factor_;
    double scale_;
    FIR<T,T_TAPS> filter_;
};

#include "detail/fir_interpolator.ipp"

using fir_interpolator_ff = FIR_INTERPOLATOR<float, float>;
using fir_interpolator_dd = FIR_INTERPOLATOR<double, double>;
using fir_interpolator_cff = FIR_INTERPOLATOR<std::complex<float>, float>;
using fir_interpolator_cdd = FIR_INTERPOLATOR<std::complex<double>, double>;

using fir_interpolator_cfcf = FIR_INTERPOLATOR<std::complex<float>, std::complex<float>>;
using fir_interpolator_cdcd = FIR_INTERPOLATOR<std::complex<double>, std::complex<double>>;

} // namespace

#endif
