#pragma once

#include "plasma/dsp/fir.hpp"
#include <vector>
#include <complex>

using namespace std;

namespace dsp {

class fir_decimator {
public:
    fir_decimator(void);
    ~fir_decimator(void) = default;

    void set_taps(vector<double> const& taps);

    void set_factor(unsigned int factor);
    unsigned int get_factor(void) const;

    void reset(void);

    vector<complex<double>> execute(vector<complex<double>> const& in);

private:
    unsigned int factor_;
    unsigned int samples_;
    fir filter_;
};

} // namespace
