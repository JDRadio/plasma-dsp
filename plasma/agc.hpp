////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the AmateurRadio.Engineer project
//! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
//! \date 2020
//! \copyright AmateurRadio.Engineer
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_AGC_HPP
#define PLASMA_AGC_HPP

#include <complex>
#include <cmath>
#include <vector>

namespace plasma {

template <typename T>
class AGC {
public:
    AGC(void);
    ~AGC(void) = default;

    double get_max_gain(void) const;
    void set_max_gain(double max_gain);

    double get_min_gain(void) const;
    void set_min_gain(double min_gain);

    void set_gain_limits(double min_gain, double max_gain);

    double get_scale(void) const;
    void set_scale(double scale);

    double get_bandwidth(void) const;
    void set_bandwidth(double bandwidth);

    double get_rssi(void) const;
    void set_rssi(double rssi);

    double get_gain(void) const;
    void set_gain(double gain);

    void lock(void);
    void unlock(void);
    bool locked(void) const;

    T execute(T x);
    std::vector<T> execute(std::vector<T> const& in);

private:
    void internal_execute(double energy);
    void clamp_gain(void);

    double max_gain_;   //!< maximum gain
    double min_gain_;   //!< minimum gain
    double gain_;       //!< current gain
    double scale_;      //!< output scale
    double alpha_;      //!< feedback gain
    double energy_;     //!< signal energy estimate
    bool locked_;       //!< locked
};

#include "detail/agc.ipp"

using agc_f = AGC<float>;
using agc_d = AGC<double>;
using agc_cf = AGC<std::complex<float>>;
using agc_cd = AGC<std::complex<double>>;

} // namespace

#endif
