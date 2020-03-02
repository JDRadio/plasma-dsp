#pragma once

#include <complex>
#include <vector>

using namespace std;

namespace dsp {

class qpsk {
public:
    qpsk(void) = default;
    ~qpsk(void) = default;

    vector<complex<double>> modulate(string const& in);
    complex<double> modulate(unsigned int symbol);

    unsigned int demodulate(complex<double> sample);
    string demodulate(vector<complex<double>> const& in);
};

} // namespace
