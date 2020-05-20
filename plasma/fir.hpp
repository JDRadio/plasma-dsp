////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the AmateurRadio.Engineer project
//! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
//! \date 2020
//! \copyright AmateurRadio.Engineer
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_FIR_HPP
#define PLASMA_FIR_HPP

#include <vector>
#include <complex>

namespace plasma {

template <typename T, typename T_TAPS = double>
class FIR {
public:
    FIR(void) = default;
    ~FIR(void) = default;

    void set_taps(std::vector<T_TAPS> const& taps);

    unsigned int get_delay(void) const;

    void reset(void);

    void push(T x);
    T execute(void);

    std::vector<T> execute(std::vector<T> const& in);

private:
    std::vector<T_TAPS> taps_;
    std::vector<T> samples_;
};

#include "detail/fir.ipp"

using fir_ff = FIR<float, float>;
using fir_dd = FIR<double, double>;
using fir_cff = FIR<std::complex<float>, float>;
using fir_cdd = FIR<std::complex<double>, double>;

using fir_cfcf = FIR<std::complex<float>, std::complex<float>>;
using fir_cdcd = FIR<std::complex<double>, std::complex<double>>;

} // namespace

#endif
