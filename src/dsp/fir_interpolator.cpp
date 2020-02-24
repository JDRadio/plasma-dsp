#include "plasma/dsp/fir_interpolator.hpp"
#include "plasma/dsp/fir_designer.hpp"

namespace dsp {

fir_interpolator::fir_interpolator(void)
    : factor_(1)
    , scale_(1)
    , filter_()
{
}

void fir_interpolator::set_factor(unsigned int factor)
{
    factor_ = factor;

    scale_ = factor_;
    filter_.set_taps(fir_designer::create_kaiser(0.5 / factor_ - 0.025, 0.05, 60.0));
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

vector<complex<double>> fir_interpolator::execute_complex(vector<complex<double>> const& in)
{
    if (factor_ == 1) {
        return in;
    }

    vector<complex<double>> out;
    out.reserve(in.size() * factor_);

    for (size_t i = 0; i < in.size(); i++) {
        filter_.push_complex(in[i]);
        out.push_back(scale_ * filter_.execute_complex());

        for (unsigned int k = 1; k < factor_; k++) {
            filter_.push_complex(0);
            out.push_back(scale_ * filter_.execute_complex());
        }
    }

    return out;
}

} // namespace
