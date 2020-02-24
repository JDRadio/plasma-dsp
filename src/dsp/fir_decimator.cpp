#include "plasma/dsp/fir_decimator.hpp"
#include "plasma/dsp/fir_designer.hpp"

namespace dsp {

fir_decimator::fir_decimator(void)
    : factor_(1)
    , samples_(0)
    , filter_()
{
}

void fir_decimator::set_factor(unsigned int factor)
{
    factor_ = factor;
    samples_ = 0;
    filter_.set_taps(fir_designer::create_kaiser(0.5 / factor_ - 0.025, 0.05, 60.0));
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

vector<double> fir_decimator::execute(vector<double> const& in)
{
    if (factor_ == 1) {
        return in;
    }

    vector<double> out;
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

vector<complex<double>> fir_decimator::execute_complex(vector<complex<double>> const& in)
{
    if (factor_ == 1) {
        return in;
    }

    vector<complex<double>> out;
    out.reserve(in.size() / factor_ + 2);

    for (size_t i = 0; i < in.size(); i++) {
        filter_.push_complex(in[i]);

        if (samples_++ % factor_ == 0) {
            out.push_back(filter_.execute_complex());
            samples_ %= factor_;
        }
    }

    return out;
}

} // namespace
