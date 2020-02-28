#pragma once

#include "nco.hpp"
#include <complex>

using namespace std;

namespace dsp {

class chirp
{
public:
    chirp(void);
    ~chirp(void) = default;

    double execute(void);
    complex<double> execute_complex(void);

    void step(void);

    void set_parameters(double sample_rate, double start, double end, double time);

private:
    double sample_rate_;
    double freq_start_;
    double freq_end_;
    double freq_adjust_;
    double sweep_time_;
    nco nco_;
};

} // namespace
