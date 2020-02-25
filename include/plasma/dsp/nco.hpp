#pragma once

#include <complex>
#include <vector>

using namespace std;

namespace dsp {

class nco {
public:
    nco(void);
    ~nco(void) = default;

    double get_angular_frequency(void) const;       //!< [rad/s]
    void set_angular_frequency(double frequency);   //!< [rad/s]
    void adjust_angular_frequency(double dw);       //!< [rad/s]

    double get_frequency(void) const;               //!< [Hz]
    void set_frequency(double frequency);           //!< [Hz]
    void adjust_frequency(double df);               //!< [Hz]

    double get_phase(void) const;   //!< [rad]
    void set_phase(double phase);   //!< [rad]
    void adjust_phase(double dp);   //!< [rad]

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
