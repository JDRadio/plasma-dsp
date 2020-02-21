#include "plasma-dsp/fir.hpp"

namespace dsp {

fir::fir(vector<float> const& taps)
    : taps_(taps.size())
    , samples_(taps.size(), 0)
{
    for (size_t i = 0; i < taps.size(); i++) {
        taps_[i] = taps[i];
    }
}

fir::fir(vector<complex<float>> const& taps)
    : taps_(taps)
    , samples_(taps.size(), 0)
{
}

void fir::set_taps(vector<float> const& taps)
{
    taps_.clear();
    taps_.resize(taps.size());

    for (size_t i = 0; i < taps.size(); i++) {
        taps_[i] = taps[taps.size() - i - 1];
    }

    samples_.clear();
    samples_.resize(taps_.size(), 0);
}

void fir::set_complex_taps(vector<complex<float>> const& taps)
{
    taps_.clear();
    taps_.insert(taps_.end(), taps.rbegin(), taps.rend());
    samples_.clear();
    samples_.resize(taps_.size(), 0);
}

void fir::push(float x)
{
    samples_.erase(samples_.begin());
    samples_.push_back(complex<float>(x, 0));
}

float fir::execute(void)
{
    complex<float> v = 0.f;

    for (size_t i = 0; i < taps_.size(); i++) {
        v += taps_[i] * samples_[i];
    }

    return real(v);
}

void fir::push_complex(complex<float> x)
{
    samples_.erase(samples_.begin());
    samples_.push_back(x);
}

complex<float> fir::execute_complex(void)
{
    complex<float> v = 0.f;

    for (size_t i = 0; i < taps_.size(); i++) {
        v += taps_[i] * samples_[i];
    }

    return v;
}

} // namespace
