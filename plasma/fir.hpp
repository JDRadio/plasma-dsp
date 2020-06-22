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

//! Finite Impulse Response (FIR) filter
template <typename T, typename T_TAPS>
class FIR {
public:
    FIR(void);
    ~FIR(void) = default;

    void set_taps(std::vector<T_TAPS> const& taps);

    unsigned int get_delay(void) const;

    void reset(void);

    //! Push a sample to the filter
    void push(T x);

    //! Calculate the filter output
    T execute(void);

    //! Push samples and calculate the filter output after each one
    std::vector<T> execute(std::vector<T> const& in);

private:
    //! Filter taps
    std::vector<T_TAPS> taps_;

    //! Samples inside the filter
    std::vector<T> samples_;

    //! Circular buffer write position
    std::size_t head_;
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
