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

double chirp::execute(void)
{
    return real(this->execute_complex());
}

complex<double> chirp::execute_complex(void)
{
    return nco_.mix_up_complex(complex<double>(1, 0));
}

void chirp::step(void)
{
    nco_.step();
    nco_.adjust_frequency(freq_adjust_);

    if (nco_.get_frequency() >= freq_end_) {
        nco_.set_frequency(freq_start_);
    }
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
