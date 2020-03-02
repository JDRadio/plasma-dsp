#include "plasma/dsp/qpsk.hpp"
#include "plasma/math.hpp"
#include <cmath>
#include<iostream>

namespace plasma::dsp {

vector<complex<double>> qpsk::modulate(string const& in)
{
    vector<complex<double>> out(in.size() * 4);

    for (size_t i = 0; i < in.size(); i++) {
        out[i*4+0] = this->modulate(in[i] >> 6);
        out[i*4+1] = this->modulate(in[i] >> 4);
        out[i*4+2] = this->modulate(in[i] >> 2);
        out[i*4+3] = this->modulate(in[i]);
    }

    return out;
}

complex<double> qpsk::modulate(unsigned int symbol)
{
    int i = (symbol & 1) == 0 ? -1 : 1;
    int q = ((symbol >> 1) & 1) == 0 ? -1 : 1;

    return complex<double>(math::sqrt1_2 * i, math::sqrt1_2 * q);
}

string qpsk::demodulate(vector<complex<double>> const& in)
{
    size_t count = in.size() / 4 + (in.size() % 4 > 0 ? 1 : 0);
    string out;
    out.reserve(count);

    for (size_t i = 0; i < count; i++) {
        int a = demodulate(in[i*4+0]);
        int b = 0;
        int c = 0;
        int d = 0;

        if (i + 1 < in.size()) {
            b = demodulate(in[i*4+1]);
        }

        if (i + 2 < in.size()) {
            c = demodulate(in[i*4+2]);
        }

        if (i + 3 < in.size()) {
            d = demodulate(in[i*4+3]);
        }

        out.push_back((a << 6) | (b << 4) | (c << 2) | d);
    }

    return out;
}

unsigned int qpsk::demodulate(complex<double> sample)
{
    int i = sample.real() < 0 ? 0 : 1;
    int q = sample.imag() < 0 ? 0 : 1;

    return (q << 1) | i;
}

} // namespace
