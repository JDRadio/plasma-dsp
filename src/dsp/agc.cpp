#include "plasma/dsp/agc.hpp"
#include <cmath>

namespace plasma::dsp {

agc::agc(void)
    : max_gain_(1e6)
    , min_gain_(1e-6)
    , gain_(1.0)
    , scale_(1.0)
    , alpha_(0.01)
    , energy_(1.0)
    , locked_(false)
{
}

double agc::get_max_gain(void) const
{
    return max_gain_;
}

void agc::set_max_gain(double max_gain)
{
    max_gain_ = max_gain;
    this->clamp_gain();
}

double agc::get_min_gain(void) const
{
    return min_gain_;
}

void agc::set_min_gain(double min_gain)
{
    min_gain_ = min_gain;
    this->clamp_gain();
}

void agc::set_gain_limits(double min_gain, double max_gain)
{
    min_gain_ = min_gain;
    max_gain_ = max_gain;
    this->clamp_gain();
}

double agc::get_scale(void) const
{
    return scale_;
}

void agc::set_scale(double scale)
{
    scale_ = scale;
}

double agc::get_bandwidth(void) const
{
    return alpha_;
}

void agc::set_bandwidth(double bandwidth)
{
    alpha_ = bandwidth;
}

double agc::get_rssi(void) const
{
    return -20. * log10(gain_);
}

void agc::set_rssi(double rssi)
{
    this->set_gain(pow(10., -rssi / 20.));
}

double agc::get_gain(void) const
{
    return gain_;
}

void agc::set_gain(double gain)
{
    gain_ = gain;
    this->clamp_gain();
}

void agc::lock(void)
{
    locked_ = true;
}

void agc::unlock(void)
{
    locked_ = false;
}

bool agc::locked(void) const
{
    return locked_;
}

void agc::internal_execute(double energy)
{
    energy_ = (1. - alpha_) * energy_ + alpha_ * energy;

    if (locked_) {
        return;
    }

    if (energy_ > 1e-16) {
        gain_ *= pow(energy_, -0.5 * alpha_);
    }

    this->clamp_gain();
}

complex<double> agc::execute(complex<double> x)
{
    complex<double> y = x * gain_;
    this->internal_execute(abs(y) * abs(y));
    return y * scale_;
}

vector<complex<double>> agc::execute(vector<complex<double>> const& in)
{
    vector<complex<double>> out = in;

    for (auto& y : out) {
        y *= gain_;
        this->internal_execute(abs(y) * abs(y));
        y *= scale_;
    }

    return out;
}

void agc::clamp_gain(void)
{
    if (gain_ > max_gain_) {
        gain_ = max_gain_;
    }
    else if (gain_ < min_gain_) {
        gain_ = min_gain_;
    }
}

} // namespace
