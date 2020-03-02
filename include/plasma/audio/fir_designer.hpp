#pragma once

#include "fir.hpp"
#include "fir_interpolator.hpp"
#include "fir_decimator.hpp"
#include <vector>

using namespace std;

namespace plasma::audio {

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
};

} // namespace
