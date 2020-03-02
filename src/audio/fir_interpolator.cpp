#include "plasma/audio/fir_interpolator.hpp"

namespace plasma::audio {

fir_interpolator::fir_interpolator(void)
    : factor_(1)
    , scale_(1)
    , filter_()
{
}

void fir_interpolator::set_taps(vector<double> const& taps)
{
    filter_.set_taps(taps);
}

void fir_interpolator::set_factor(unsigned int factor)
{
    factor_ = factor;

    scale_ = factor_;
}

unsigned int fir_interpolator::get_factor(void) const
{
    return factor_;
}

unsigned int fir_interpolator::get_delay(void) const
{
    return filter_.get_delay();
}

void fir_interpolator::reset(void)
{
    filter_.reset();
}

vector<double> fir_interpolator::execute(vector<double> const& in)
{
    if (factor_ == 1) {
        return in;
    }

    vector<double> out;
    out.reserve(in.size() * factor_);

    for (size_t i = 0; i < in.size(); i++) {
        filter_.push(in[i]);
        out.push_back(scale_ * filter_.execute());

        for (unsigned int k = 1; k < factor_; k++) {
            filter_.push(0);
            out.push_back(scale_ * filter_.execute());
        }
    }

    return out;
}

} // namespace
