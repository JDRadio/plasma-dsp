////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the AmateurRadio.Engineer project
//! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
//! \date 2020
//! \copyright AmateurRadio.Engineer
////////////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include <functional>

namespace plasma {

template <typename T, typename TC>
SQUELCH<T,TC>::SQUELCH(void)
    : alpha_(0.1)
    , energy_(0.)
    , locked_(false)
    , threshold_(-60.)
    , timeout_(100)
    , timer_(0)
    , status_(status::on)
{
}

template <typename T, typename TC>
void SQUELCH<T,TC>::set_status(status s)
{
    status_ = s;
}

template <typename T, typename TC>
typename SQUELCH<T,TC>::status SQUELCH<T,TC>::get_status(void) const
{
    return status_;
}

template <typename T, typename TC>
bool SQUELCH<T,TC>::squelched(void) const
{
    return status_ == status::on;
}

template <typename T, typename TC>
T SQUELCH<T,TC>::get_threshold(void) const
{
    return threshold_;
}

template <typename T, typename TC>
void SQUELCH<T,TC>::set_threshold(T threshold)
{
    threshold_ = threshold;
}

template <typename T, typename TC>
unsigned int SQUELCH<T,TC>::get_timeout(void) const
{
    return timeout_;
}

template <typename T, typename TC>
void SQUELCH<T,TC>::set_timeout(unsigned int timeout)
{
    timeout_ = timeout;
}

template <typename T, typename TC>
T SQUELCH<T,TC>::get_bandwidth(void) const
{
    return alpha_;
}

template <typename T, typename TC>
void SQUELCH<T,TC>::set_bandwidth(T bandwidth)
{
    alpha_ = bandwidth;
}

template <typename T, typename TC>
T SQUELCH<T,TC>::get_rssi(void) const
{
    return 10. * std::log10(energy_);
}

template <typename T, typename TC>
void SQUELCH<T,TC>::set_rssi(T rssi)
{
    energy_ = std::pow(10., rssi / 10.);
}

template <typename T, typename TC>
void SQUELCH<T,TC>::lock(void)
{
    locked_ = true;
}

template <typename T, typename TC>
void SQUELCH<T,TC>::unlock(void)
{
    locked_ = false;
}

template <typename T, typename TC>
bool SQUELCH<T,TC>::locked(void) const
{
    return locked_;
}

template <typename T, typename TC>
typename SQUELCH<T,TC>::status SQUELCH<T,TC>::internal_execute(T energy)
{
    energy_ = (1. - alpha_) * energy_ + alpha_ * energy;

    if (locked_) {
        return status_;
    }

    bool above_threshold = get_rssi() > threshold_;

    if (above_threshold) {
        status_ = status::off;
    }
    else {
        if (status_ == status::off) {
            status_ = status::holding;
            timer_ = timeout_;
        }
        else if (status_ == status::holding) {
            timer_--;

            if (timer_ == 0) {
                status_ = status::on;
            }
        }
    }

    return status_;
}

template <typename T, typename TC>
typename SQUELCH<T,TC>::status SQUELCH<T,TC>::execute(TC x)
{
    return internal_execute(std::abs(x * std::conj(x)));
}

template <typename T, typename TC>
void SQUELCH<T,TC>::execute(std::vector<TC> const& in, std::vector<status>& out)
{
    out.resize(in.size());

    for (std::size_t n = 0; n < in.size(); ++n) {
        out[n] = execute(in[n]);
    }
}

template <typename T, typename TC>
void SQUELCH<T,TC>::execute(std::vector<TC> const& in)
{
    std::for_each(in.cbegin(), in.cend(), std::bind(execute, this, std::placeholders::_1));
}

} // namespace
