////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the AmateurRadio.Engineer project
//! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
//! \date 2020
//! \copyright AmateurRadio.Engineer
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_FIR_DESIGNER_HPP
#define PLASMA_FIR_DESIGNER_HPP

#include "fir.hpp"
#include "fir_interpolator.hpp"
#include "fir_decimator.hpp"
#include "math.hpp"
#include <vector>
#include <complex>
#include <cmath>

namespace plasma {

template <typename T, typename T_TAPS>
class FIR_DESIGNER {
public:
    FIR_DESIGNER(void) = delete;
    ~FIR_DESIGNER(void) = delete;

    //! Creates a Kaiser-windowed filter
    //! @param fc Cut-off frequency
    //! @param df Transition bandwidth
    //! @param att Stop-band attenuation
    static std::vector<T_TAPS> create_kaiser_taps(double fc, double df, double att);
    static FIR<T,T_TAPS> create_kaiser_filter(double fc, double df, double att);
    static FIR_INTERPOLATOR<T,T_TAPS> create_kaiser_interpolator(unsigned int k, double df, double att);
    static FIR_DECIMATOR<T,T_TAPS> create_kaiser_decimator(unsigned int k, double df, double att);

    //! Creates a root-raised-cosine filter
    //! @param k Samples per symbol
    //! @param m Symbol delay
    //! @param r Roll-off factor
    static std::vector<T_TAPS> create_rrc_taps(unsigned int k, unsigned int m, double r);
    static FIR_INTERPOLATOR<T,T_TAPS> create_rrc_interpolator(unsigned int k, unsigned int m, double r);
    static FIR_DECIMATOR<T,T_TAPS> create_rrc_decimator(unsigned int k, unsigned int m, double r);

private:
    static unsigned int kaiser_order(double att, double df);
    static double kaiser_beta(double att);
    static double kaiser_window(double beta, int n, int big_n);
};

#include "detail/fir_designer.ipp"

using fir_designer_ff = FIR_DESIGNER<float, float>;
using fir_designer_dd = FIR_DESIGNER<double, double>;
using fir_designer_cff = FIR_DESIGNER<std::complex<float>, float>;
using fir_designer_cdd = FIR_DESIGNER<std::complex<double>, double>;

} // namespace

#endif
