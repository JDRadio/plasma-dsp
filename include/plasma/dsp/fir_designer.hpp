#pragma once

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
    static vector<double> create_kaiser(double fc, double df, double att);

    /// Creates a root-raised-cosine filter
    /// @param k Samples per symbol
    /// @param m Symbol delay
    /// @param r Roll-off factor
    static vector<double> create_rrc(unsigned int k, unsigned int m, double r);

private:
    static unsigned int kaiser_order(double att, double df);
    static double kaiser_beta(double att);
    static double kaiser_window(double beta, int n, int big_n);
};

} // namespace
