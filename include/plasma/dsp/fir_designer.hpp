#pragma once

#include <vector>
#include <complex>

using namespace std;

namespace dsp {

class fir_designer {
public:
    fir_designer(void) = delete;
    ~fir_designer(void) = delete;

    static vector<double> create_kaiser(double fc, double df, double att);

private:
    static unsigned int kaiser_order(double att, double df);
    static double kaiser_beta(double att);
    static double kaiser_window(double beta, int n, int big_n);
};

} // namespace
