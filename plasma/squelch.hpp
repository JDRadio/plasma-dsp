////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the AmateurRadio.Engineer project
//! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
//! \date 2020
//! \copyright AmateurRadio.Engineer
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_SQUELCH_HPP
#define PLASMA_SQUELCH_HPP

#include <vector>
#include <complex>

namespace plasma {

template <typename T, typename TC>
class SQUELCH {
public:
    enum class status {
        off,        //!< no squelch, signal is strong
        on,         //!< squelched, signal is weak
        holding     //!< holding, signal is falling
    };

    SQUELCH(void);
    ~SQUELCH(void) = default;

    void set_status(status s);
    status get_status(void) const;
    bool squelched(void) const;

    T get_threshold(void) const;
    void set_threshold(T threshold);

    unsigned int get_timeout(void) const;
    void set_timeout(unsigned int timeout);

    T get_bandwidth(void) const;
    void set_bandwidth(T bandwidth);

    T get_rssi(void) const;
    void set_rssi(T rssi);

    void lock(void);
    void unlock(void);
    bool locked(void) const;

    status execute(TC x);
    std::vector<status> execute(std::vector<TC> const& in);

private:
    status internal_execute(T energy);

    T alpha_;          //!< feedback gain for lowpass filter
    T energy_;         //!< signal energy estimate
    bool locked_;           //!< locked
    T threshold_;      //!< energy threshold in dB
    unsigned int timeout_;  //!< number of samples below threshold before turning on
    unsigned int timer_;    //!< timer in samples below threshold
    status status_;         //!< current status
};

#include "detail/squelch.ipp"

using squelch_f = SQUELCH<float, float>;
using squelch_d = SQUELCH<double, double>;
using squelch_cf = SQUELCH<float, std::complex<float>>;
using squelch_cd = SQUELCH<double, std::complex<double>>;

} // namespace

#endif
