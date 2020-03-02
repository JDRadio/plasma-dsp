#pragma once

#include "nco.hpp"
#include <complex>
#include <vector>

using namespace std;

namespace plasma::dsp {

class chirp
{
public:
    chirp(void);
    ~chirp(void) = default;

    void step(void);

    /// Generates a chirp sample
    complex<double> execute(void);

    /// Generates multiple chirp samples
    vector<complex<double>> execute(size_t count);

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
