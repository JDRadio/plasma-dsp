#include "plasma/dsp/squelch.hpp"
#include <cmath>

namespace plasma::dsp {

squelch::squelch(void)
    : alpha_(0.1)
    , energy_(0.)
    , locked_(false)
    , threshold_(-60.)
    , timeout_(100)
    , timer_(0)
    , status_(status::on)
{
}

void squelch::set_status(status s)
{
    status_ = s;
}

squelch::status squelch::get_status(void) const
{
    return status_;
}

bool squelch::squelched(void) const
{
    return status_ == status::on;
}

double squelch::get_threshold(void) const
{
    return threshold_;
}

void squelch::set_threshold(double threshold)
{
    threshold_ = threshold;
}

unsigned int squelch::get_timeout(void) const
{
    return timeout_;
}

void squelch::set_timeout(unsigned int timeout)
{
    timeout_ = timeout;
}

double squelch::get_bandwidth(void) const
{
    return alpha_;
}

void squelch::set_bandwidth(double bandwidth)
{
    alpha_ = bandwidth;
}

double squelch::get_rssi(void) const
{
    return 10. * log10(energy_);
}

void squelch::set_rssi(double rssi)
{
    energy_ = pow(10., rssi / 10.);
}

void squelch::lock(void)
{
    locked_ = true;
}

void squelch::unlock(void)
{
    locked_ = false;
}

bool squelch::locked(void) const
{
    return locked_;
}

squelch::status squelch::internal_execute(double energy)
{
    energy_ = (1. - alpha_) * energy_ + alpha_ * energy;

    if (locked_) {
        return status_;
    }

    bool above_threshold = this->get_rssi() > threshold_;

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

squelch::status squelch::execute(complex<double> x)
{
    return this->internal_execute(abs(x) * abs(x));
}

vector<squelch::status> squelch::execute(vector<complex<double>> const& in)
{
    vector<status> out(in.size());

    for (size_t i = 0; i < in.size(); i++) {
        out[i] = this->execute(in[i]);
    }

    return out;
}

} // namespace
