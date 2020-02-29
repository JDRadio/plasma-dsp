#include "plasma/dsp/qpsk.hpp"
#include "plasma/dsp/math.hpp"

namespace dsp {

complex<double> qpsk::modulate(unsigned int symbol)
{
    int i = (symbol & 1) == 0 ? -1 : 1;
    int q = ((symbol >> 1) & 1) == 0 ? -1 : 1;

    return complex<double>(math::sqrt1_2 * i, math::sqrt1_2 * q);
}

unsigned int qpsk::demodulate(complex<double> sample)
{
    int i = sample.real() < 0 ? -1 : 1;
    int q = sample.imag() < 0 ? -1 : 1;

    return (q << 1) | i;
}

} // namespace
