#include "plasma/dsp/chirp.hpp"

namespace dsp {

chirp::chirp(void)
    : sample_rate_(0)
    , freq_start_(0)
    , freq_end_(0)
    , freq_adjust_(0)
    , sweep_time_(0)
    , nco_()
{
}

complex<double> chirp::execute(void)
{
    return nco_.mix_up(complex<double>(1, 0));
}

vector<complex<double>> chirp::execute(size_t count)
{
    vector<complex<double>> out(count);

    for (size_t i = 0; i < count; i++) {
        out[i] = this->execute();
        this->step();
    }

    return out;
}

void chirp::step(void)
{
    nco_.adjust_frequency(freq_adjust_);

    if (nco_.get_frequency() >= freq_end_) {
        nco_.set_frequency(freq_start_);
    }

    nco_.step();
}

void chirp::set_parameters(double sample_rate, double start, double end, double time)
{
    sample_rate_ = sample_rate;
    freq_start_ = start / sample_rate_;
    freq_end_ = end / sample_rate_;
    sweep_time_ = time;

    freq_adjust_ = (freq_end_ - freq_start_) / (sweep_time_ * sample_rate_);
    nco_.set_frequency(freq_start_);
}

} // namespace
