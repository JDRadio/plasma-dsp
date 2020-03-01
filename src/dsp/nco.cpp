#include "plasma/dsp/nco.hpp"
#include "plasma/dsp/math.hpp"
#include <cmath>

namespace dsp {

nco::nco(void)
    : theta_(0)
    , d_theta_(0)
{
}

double nco::get_angular_frequency(void) const
{
    return d_theta_;
}

double nco::get_frequency(void) const
{
    return d_theta_ / (2.0 * math::pi);
}

void nco::set_angular_frequency(double frequency)
{
    d_theta_ = frequency;
}

void nco::set_frequency(double frequency)
{
    d_theta_ = 2.0 * math::pi * frequency;
}

void nco::adjust_angular_frequency(double dw)
{
    d_theta_ += dw;
}

void nco::adjust_frequency(double df)
{
    d_theta_ += 2.0 * math::pi * df;
}

double nco::get_phase(void) const
{
    return theta_;
}

void nco::set_phase(double phase)
{
    theta_ = fmod(phase, math::two_pi);
}

void nco::adjust_phase(double dp)
{
    theta_ = fmod(theta_ + dp, math::two_pi);
}

void nco::step(void)
{
    theta_ = fmod(theta_ + d_theta_, math::two_pi);
}

void nco::reset(void)
{
    theta_ = 0;
}

complex<double> nco::mix_up(complex<double> x)
{
    return x * exp(complex<double>(0, theta_));
}

complex<double> nco::mix_down(complex<double> x)
{
    return x * exp(complex<double>(0, -theta_));
}

vector<complex<double>> nco::mix_up(vector<complex<double>> const& in)
{
    vector<complex<double>> out(in.size());

    for (size_t i = 0; i < in.size(); i++) {
        out[i] = this->mix_up(in[i]);
        this->step();
    }

    return out;
}

vector<complex<double>> nco::mix_down(vector<complex<double>> const& in)
{
    vector<complex<double>> out(in.size());

    for (size_t i = 0; i < in.size(); i++) {
        out[i] = this->mix_down(in[i]);
        this->step();
    }

    return out;
}

} // namespace
