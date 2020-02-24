#pragma once

#include <complex>
#include <vector>

using namespace std;

namespace dsp {

class nco {
public:
    nco(void);
    ~nco(void) = default;

    double get_frequency(void) const;
    void set_frequency(double frequency);
    void adjust_frequency(double df);

    double get_phase(void) const;
    void set_phase(double phase);
    void adjust_phase(double dp);

    void step(void);
    void reset(void);

    double mix_up(double x);
    double mix_down(double x);

    complex<double> mix_up_complex(complex<double> x);
    complex<double> mix_down_complex(complex<double> x);

private:
    double theta_;
    double d_theta_;
};

} // namespace
