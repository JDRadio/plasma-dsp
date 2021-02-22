////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the AmateurRadio.Engineer project
//! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
//! \date 2020
//! \copyright AmateurRadio.Engineer
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_NCO_TABLE_TABLE_HPP
#define PLASMA_NCO_TABLE_TABLE_HPP

#include "math.hpp"
#include <complex>
#include <vector>
#include <array>
#include <cmath>
#include <cstdint>

namespace plasma {

template <typename T>
class NCO_TABLE {
public:
    NCO_TABLE(void);
    ~NCO_TABLE(void) = default;

    double get_angular_frequency(void) const;   //!< [rad/s]
    void set_angular_frequency(double w);       //!< [rad/s]
    void adjust_angular_frequency(double dw);   //!< [rad/s]

    double get_frequency(void) const;           //!< [Hz]
    void set_frequency(double frequency);       //!< [Hz]
    void adjust_frequency(double df);           //!< [Hz]

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
    static constexpr int SAMPLE_COUNT_BITS = 10;
    static constexpr int SAMPLE_COUNT = 1 << SAMPLE_COUNT_BITS;

    std::complex<T> get_value(void) const;
    std::uint32_t constrain_value(double val) const;


    //! Phase [rad]
    std::uint32_t theta_;

    //! Frequency [rad/sample]
    std::uint32_t d_theta_;

    //! PLL frequency proportion
    double alpha_;

    //! PLL phase proportion
    double beta_;

    //! Sample table
    std::array<std::complex<T>, SAMPLE_COUNT> table_;
};

#include "detail/nco_table.ipp"

using nco_table_cf = NCO_TABLE<float>;
using nco_table_cd = NCO_TABLE<double>;

} // namespace

#endif
