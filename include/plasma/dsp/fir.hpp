#pragma once

#include <vector>
#include <complex>

using namespace std;

namespace dsp {

class fir {
public:
    fir(void) = default;
    ~fir(void) = default;

    void set_taps(vector<double> const& taps);
    void set_complex_taps(vector<complex<double>> const& taps);

    void set_tapsf(vector<float> const& taps);
    void set_complex_tapsf(vector<complex<float>> const& taps);

    void push(double x);
    double execute(void);

    void push_complex(complex<double> x);
    complex<double> execute_complex(void);

private:
    vector<complex<double>> taps_;
    vector<complex<double>> samples_;
};

} // namespace
