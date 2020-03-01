#pragma once

#include "plasma/dsp/fir.hpp"
#include <vector>
#include <complex>

using namespace std;

namespace dsp {

class fir_interpolator {
public:
    fir_interpolator(void);
    ~fir_interpolator(void) = default;

    void set_factor(unsigned int factor);
    unsigned int get_factor(void) const;

    unsigned int get_delay(void) const;

    void reset(void);

    vector<complex<double>> execute(vector<complex<double>> const& in);

private:
    unsigned int factor_;
    double scale_;
    fir filter_;
};

} // namespace
