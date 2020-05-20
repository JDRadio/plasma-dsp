////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the AmateurRadio.Engineer project
//! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
//! \date 2020
//! \copyright AmateurRadio.Engineer
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_NCO_HPP
#define PLASMA_NCO_HPP

#include "math.hpp"
#include <complex>
#include <vector>
#include <cmath>

namespace plasma {

template <typename T>
class NCO {
public:
    NCO(void);
    ~NCO(void) = default;

    double get_angular_frequency(void) const;       //!< [rad/s]
    void set_angular_frequency(double frequency);   //!< [rad/s]
    void adjust_angular_frequency(double dw);       //!< [rad/s]

    double get_frequency(void) const;               //!< [Hz]
    void set_frequency(double frequency);           //!< [Hz]
    void adjust_frequency(double df);               //!< [Hz]

    double get_phase(void) const;   //!< [rad]
    void set_phase(double phase);   //!< [rad]
    void adjust_phase(double dp);   //!< [rad]

    void set_pll_bandwidth(double bandwidth);
    void pll_step(double dphi);

    void step(void);
    void reset(void);

    std::complex<T> mix_up(std::complex<T> x);
    std::vector<std::complex<T>> mix_up(std::vector<std::complex<T>> const& in);
    std::complex<T> mix_down(std::complex<T> x);
    std::vector<std::complex<T>> mix_down(std::vector<std::complex<T>> const& in);

private:
    //! Phase [rad]
    double theta_;

    //! Frequency [rad/sample]
    double d_theta_;

    //! PLL frequency proportion
    double alpha_;

    //! PLL phase proportion
    double beta_;
};

#include "detail/nco.ipp"

using nco_f = NCO<float>;
using nco_d = NCO<double>;

} // namespace

#endif
