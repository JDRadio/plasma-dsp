////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the AmateurRadio.Engineer project
//! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
//! \date 2020
//! \copyright AmateurRadio.Engineer
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_CHIRP_HPP
#define PLASMA_CHIRP_HPP

#include "nco.hpp"
#include <complex>
#include <vector>

namespace plasma {

template <typename T>
class CHIRP {
public:
    CHIRP(void);
    ~CHIRP(void) = default;

    void step(void);

    //! Generates a chirp sample
    std::complex<T> execute(void);

    //! Generates multiple chirp samples
    std::vector<std::complex<T>> execute(std::size_t count);

    void set_parameters(double sample_rate, double start, double end, double time);

private:
    double sample_rate_;
    double freq_start_;
    double freq_end_;
    double freq_adjust_;
    double sweep_time_;
    NCO<T> nco_;
};

#include "detail/chirp.ipp"

using chirp_f = CHIRP<float>;
using chirp_d = CHIRP<double>;

} // namespace

#endif
