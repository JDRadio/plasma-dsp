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

    unsigned int get_delay(void) const;

    void reset(void);

    void push(complex<double> x);
    complex<double> execute(void);

    vector<complex<double>> execute(vector<complex<double>> const& in);

private:
    vector<complex<double>> taps_;
    vector<complex<double>> samples_;
};

} // namespace
