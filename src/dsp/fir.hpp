#pragma once

#include <vector>
#include <complex>

using namespace std;

namespace dsp {

class fir {
public:
    fir(void) = default;
    fir(vector<float> const& taps);
    fir(vector<complex<float>> const& taps);
    ~fir(void) = default;

    void set_taps(vector<float> const& taps);
    void set_complex_taps(vector<complex<float>> const& taps);

    void push(float x);
    float execute(void);

    void push_complex(complex<float> x);
    complex<float> execute_complex(void);

private:
    vector<complex<float>> taps_;
    vector<complex<float>> samples_;
};

} // namespace
