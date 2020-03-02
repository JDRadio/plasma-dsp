#include "plasma/dsp/fir_decimator.hpp"
#include "plasma/dsp/fir_designer.hpp"

namespace dsp {

fir_decimator::fir_decimator(void)
    : factor_(1)
    , samples_(0)
    , filter_()
{
}

void fir_decimator::set_taps(vector<double> const& taps)
{
    filter_.set_taps(taps);
}

void fir_decimator::set_factor(unsigned int factor)
{
    factor_ = factor;
    samples_ = 0;
}

unsigned int fir_decimator::get_factor(void) const
{
    return factor_;
}

void fir_decimator::reset(void)
{
    samples_ = 0;
    filter_.reset();
}

vector<complex<double>> fir_decimator::execute(vector<complex<double>> const& in)
{
    if (factor_ == 1) {
        return in;
    }

    vector<complex<double>> out;
    out.reserve(in.size() / factor_ + 2);

    for (size_t i = 0; i < in.size(); i++) {
        filter_.push(in[i]);

        if (samples_++ % factor_ == 0) {
            out.push_back(filter_.execute());
            samples_ %= factor_;
        }
    }

    return out;
}

} // namespace
