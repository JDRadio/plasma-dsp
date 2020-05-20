////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the AmateurRadio.Engineer project
//! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
//! \date 2020
//! \copyright AmateurRadio.Engineer
////////////////////////////////////////////////////////////////////////////////
template <typename T>
AGC<T>::AGC(void)
    : max_gain_(1e6)
    , min_gain_(1e-6)
    , gain_(1.0)
    , scale_(1.0)
    , alpha_(0.01)
    , energy_(1.0)
    , locked_(false)
{
}

template <typename T>
double AGC<T>::get_max_gain(void) const
{
    return max_gain_;
}

template <typename T>
void AGC<T>::set_max_gain(double max_gain)
{
    max_gain_ = max_gain;
    clamp_gain();
}

template <typename T>
double AGC<T>::get_min_gain(void) const
{
    return min_gain_;
}

template <typename T>
void AGC<T>::set_min_gain(double min_gain)
{
    min_gain_ = min_gain;
    clamp_gain();
}

template <typename T>
void AGC<T>::set_gain_limits(double min_gain, double max_gain)
{
    min_gain_ = min_gain;
    max_gain_ = max_gain;
    clamp_gain();
}

template <typename T>
double AGC<T>::get_scale(void) const
{
    return scale_;
}

template <typename T>
void AGC<T>::set_scale(double scale)
{
    scale_ = scale;
}

template <typename T>
double AGC<T>::get_bandwidth(void) const
{
    return alpha_;
}

template <typename T>
void AGC<T>::set_bandwidth(double bandwidth)
{
    alpha_ = bandwidth;
}

template <typename T>
double AGC<T>::get_rssi(void) const
{
    return -20. * std::log10(gain_);
}

template <typename T>
void AGC<T>::set_rssi(double rssi)
{
    set_gain(std::pow(10., -rssi / 20.));
}

template <typename T>
double AGC<T>::get_gain(void) const
{
    return gain_;
}

template <typename T>
void AGC<T>::set_gain(double gain)
{
    gain_ = gain;
    clamp_gain();
}

template <typename T>
void AGC<T>::lock(void)
{
    locked_ = true;
}

template <typename T>
void AGC<T>::unlock(void)
{
    locked_ = false;
}

template <typename T>
bool AGC<T>::locked(void) const
{
    return locked_;
}

template <typename T>
void AGC<T>::internal_execute(double energy)
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

template <typename T>
T AGC<T>::execute(T x)
{
    T y = x * gain_;
    internal_execute(y * std::conj(y));
    return y * scale_;
}

template <typename T>
std::vector<T> AGC<T>::execute(std::vector<T> const& in)
{
    std::vector<T> out = in;

    for (auto& y : out) {
        y *= gain_;
        internal_execute(y * std::conj(y));
        y *= scale_;
    }

    return out;
}

template <typename T>
void AGC<T>::clamp_gain(void)
{
    if (gain_ > max_gain_) {
        gain_ = max_gain_;
    }
    else if (gain_ < min_gain_) {
        gain_ = min_gain_;
    }
}
