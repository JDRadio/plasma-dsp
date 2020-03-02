#pragma once

#include "fir.hpp"
#include "fir_interpolator.hpp"
#include "fir_decimator.hpp"
#include <vector>
#include <complex>

using namespace std;

namespace dsp {

class fir_designer {
public:
    fir_designer(void) = delete;
    ~fir_designer(void) = delete;

    /// Creates a Kaiser-windowed filter
    /// @param fc Cut-off frequency
    /// @param df Transition bandwidth
    /// @param att Stop-band attenuation
    static vector<double> create_kaiser_taps(double fc, double df, double att);
    static fir create_kaiser_filter(double fc, double df, double att);
    static fir_interpolator create_kaiser_interpolator(unsigned int k, double df, double att);
    static fir_decimator create_kaiser_decimator(unsigned int k, double df, double att);

    /// Creates a root-raised-cosine filter
    /// @param k Samples per symbol
    /// @param m Symbol delay
    /// @param r Roll-off factor
    static vector<double> create_rrc_taps(unsigned int k, unsigned int m, double r);
    static fir_interpolator create_rrc_interpolator(unsigned int k, unsigned int m, double r);
    static fir_decimator create_rrc_decimator(unsigned int k, unsigned int m, double r);

private:
    static unsigned int kaiser_order(double att, double df);
    static double kaiser_beta(double att);
    static double kaiser_window(double beta, int n, int big_n);
};

} // namespace
