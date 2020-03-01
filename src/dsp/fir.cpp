#include "plasma/dsp/fir.hpp"

namespace dsp {

void fir::set_taps(vector<double> const& taps)
{
    taps_.clear();
    taps_.resize(taps.size());

    for (size_t i = 0; i < taps.size(); i++) {
        taps_[i] = taps[taps.size() - i - 1];
    }

    samples_.clear();
    samples_.resize(taps_.size(), 0);
}

void fir::set_complex_taps(vector<complex<double>> const& taps)
{
    taps_.clear();
    taps_.reserve(taps.size());
    taps_.insert(taps_.end(), taps.rbegin(), taps.rend());

    samples_.clear();
    samples_.resize(taps_.size(), 0);
}

unsigned int fir::get_delay(void) const
{
    return taps_.size() / 2;
}

void fir::reset(void)
{
    samples_.clear();
    samples_.resize(taps_.size(), 0);
}

void fir::push(complex<double> x)
{
    samples_.erase(samples_.begin());
    samples_.push_back(x);
}

complex<double> fir::execute(void)
{
    complex<double> v = 0;

    for (size_t i = 0; i < taps_.size(); i++) {
        v += taps_[i] * samples_[i];
    }

    return v;
}

vector<complex<double>> fir::execute(vector<complex<double>> const& in)
{
    vector<complex<double>> out(in.size());

    for (size_t i = 0; i < in.size(); i++) {
        this->push(in[i]);
        out[i] = this->execute();
    }

    return out;
}

} // namespace
