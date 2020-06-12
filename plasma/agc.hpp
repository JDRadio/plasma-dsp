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
#include <vector>

namespace plasma {

template <typename T, typename TC>
class AGC {
public:
    AGC(void);
    ~AGC(void) = default;

    T get_max_gain(void) const;
    void set_max_gain(T max_gain);

    T get_min_gain(void) const;
    void set_min_gain(T min_gain);

    void set_gain_limits(T min_gain, T max_gain);

    T get_scale(void) const;
    void set_scale(T scale);

    T get_bandwidth(void) const;
    void set_bandwidth(T bandwidth);

    T get_rssi(void) const;
    void set_rssi(T rssi);

    T get_gain(void) const;
    void set_gain(T gain);

    void lock(void);
    void unlock(void);
    bool locked(void) const;

    TC execute(TC x);
    void execute(std::vector<TC>& inout);
    void execute(std::vector<TC> const& in, std::vector<TC>& out);

private:
    void internal_execute(T energy);
    void clamp_gain(void);

    T max_gain_;   //!< maximum gain
    T min_gain_;   //!< minimum gain
    T gain_;       //!< current gain
    T scale_;      //!< output scale
    T alpha_;      //!< feedback gain
    T energy_;     //!< signal energy estimate
    bool locked_;       //!< locked
};

using agc_f = AGC<float, float>;
using agc_d = AGC<double, double>;
using agc_cf = AGC<float, std::complex<float>>;
using agc_cd = AGC<double, std::complex<double>>;

} // namespace

#include "detail/agc.ipp"

#endif
