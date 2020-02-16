#include "agc.hpp"
#include <cmath>

namespace dsp {

agc::agc(void)
    : max_gain_(1e6f)
    , min_gain_(1e-6f)
    , gain_(1.0f)
    , scale_(1.0f)
    , alpha_(0.01f)
    , energy_(1.0f)
    , locked_(false)
{
}

float agc::get_max_gain(void) const
{
    return max_gain_;
}

void agc::set_max_gain(float max_gain)
{
    max_gain_ = max_gain;
    this->clamp_gain();
}

float agc::get_min_gain(void) const
{
    return min_gain_;
}

void agc::set_min_gain(float min_gain)
{
    min_gain_ = min_gain;
    this->clamp_gain();
}

void agc::set_gain_limits(float min_gain, float max_gain)
{
    min_gain_ = min_gain;
    max_gain_ = max_gain;
    this->clamp_gain();
}

float agc::get_scale(void) const
{
    return scale_;
}

void agc::set_scale(float scale)
{
    scale_ = scale;
}

float agc::get_bandwidth(void) const
{
    return alpha_;
}

void agc::set_bandwidth(float bandwidth)
{
    alpha_ = bandwidth;
}

float agc::get_rssi(void) const
{
    return -20.f * log10(gain_);
}

void agc::set_rssi(float rssi)
{
    this->set_gain(pow(10.f, -rssi / 20.f));
}

float agc::get_gain(void) const
{
    return gain_;
}

void agc::set_gain(float gain)
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

void agc::internal_execute(float energy)
{
    energy_ = (1.f - alpha_) * energy_ + alpha_ * energy;

    if (locked_) {
        return;
    }

    if (energy_ > 1e-16f) {
        gain_ *= pow(energy_, -0.5f * alpha_);
    }

    this->clamp_gain();
}

float agc::execute(float x)
{
    float y = x * gain_;
    this->internal_execute(y * y);
    return y * scale_;
}

vector<float> agc::execute(vector<float> const& in)
{
    vector<float> out = in;

    for (auto& y : out) {
        y *= gain_;
        this->internal_execute(y * y);
        y *= scale_;
    }

    return out;
}

complex<float> agc::execute(complex<float> x)
{
    complex<float> y = x * gain_;
    this->internal_execute(abs(y) * abs(y));
    return y * scale_;
}

vector<complex<float>> agc::execute(vector<complex<float>> const& in)
{
    vector<complex<float>> out = in;

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
