////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the AmateurRadio.Engineer project
//! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
//! \date 2020
//! \copyright AmateurRadio.Engineer
////////////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include <functional>
#include <cmath>

namespace plasma {

template <typename T, typename TC>
AGC<T,TC>::AGC(void)
    : max_gain_(1e6)
    , min_gain_(1e-6)
    , gain_(1.0)
    , scale_(1.0)
    , alpha_(0.01)
    , energy_(1.0)
    , locked_(false)
{
}

template <typename T, typename TC>
T AGC<T,TC>::get_max_gain(void) const
{
    return max_gain_;
}

template <typename T, typename TC>
void AGC<T,TC>::set_max_gain(T max_gain)
{
    max_gain_ = max_gain;
    clamp_gain();
}

template <typename T, typename TC>
T AGC<T,TC>::get_min_gain(void) const
{
    return min_gain_;
}

template <typename T, typename TC>
void AGC<T,TC>::set_min_gain(T min_gain)
{
    min_gain_ = min_gain;
    clamp_gain();
}

template <typename T, typename TC>
void AGC<T,TC>::set_gain_limits(T min_gain, T max_gain)
{
    min_gain_ = min_gain;
    max_gain_ = max_gain;
    clamp_gain();
}

template <typename T, typename TC>
T AGC<T,TC>::get_scale(void) const
{
    return scale_;
}

template <typename T, typename TC>
void AGC<T,TC>::set_scale(T scale)
{
    scale_ = scale;
}

template <typename T, typename TC>
T AGC<T,TC>::get_bandwidth(void) const
{
    return alpha_;
}

template <typename T, typename TC>
void AGC<T,TC>::set_bandwidth(T bandwidth)
{
    alpha_ = bandwidth;
}

template <typename T, typename TC>
T AGC<T,TC>::get_rssi(void) const
{
    return -20. * std::log10(gain_);
}

template <typename T, typename TC>
void AGC<T,TC>::set_rssi(T rssi)
{
    set_gain(std::pow(10., -rssi / 20.));
}

template <typename T, typename TC>
T AGC<T,TC>::get_gain(void) const
{
    return gain_;
}

template <typename T, typename TC>
void AGC<T,TC>::set_gain(T gain)
{
    gain_ = gain;
    clamp_gain();
}

template <typename T, typename TC>
void AGC<T,TC>::lock(void)
{
    locked_ = true;
}

template <typename T, typename TC>
void AGC<T,TC>::unlock(void)
{
    locked_ = false;
}

template <typename T, typename TC>
bool AGC<T,TC>::locked(void) const
{
    return locked_;
}

template <typename T, typename TC>
void AGC<T,TC>::internal_execute(T energy)
{
    energy_ = (1. - alpha_) * energy_ + alpha_ * energy;

    if (locked_) {
        return;
    }

    if (energy_ > 1e-16) {
        gain_ *= std::pow(energy_, -0.5 * alpha_);
    }

    clamp_gain();
}

template <typename T, typename TC>
TC AGC<T,TC>::execute(TC x)
{
    TC y = x * gain_;
    internal_execute(std::abs(y * std::conj(y)));
    return y * scale_;
}

template <typename T, typename TC>
void AGC<T,TC>::execute(std::vector<TC> const& in, std::vector<TC>& out)
{
    out.resize(in.size());

    for (std::size_t n = 0; n < in.size(); ++n) {
        out[n] = execute(in[n]);
    }
}

template <typename T, typename TC>
void AGC<T,TC>::execute(std::vector<TC>& inout)
{
    std::for_each(inout.begin(), inout.end(), std::bind(execute, this, std::ref(std::placeholders::_1)));
}

template <typename T, typename TC>
void AGC<T,TC>::clamp_gain(void)
{
    if (gain_ > max_gain_) {
        gain_ = max_gain_;
    }
    else if (gain_ < min_gain_) {
        gain_ = min_gain_;
    }
}

} // namespace
