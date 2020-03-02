#pragma once

#include "fir.hpp"
#include <vector>

using namespace std;

namespace plasma::audio {

class fir_decimator {
public:
    fir_decimator(void);
    ~fir_decimator(void) = default;

    void set_taps(vector<double> const& taps);

    void set_factor(unsigned int factor);
    unsigned int get_factor(void) const;

    void reset(void);

    vector<double> execute(vector<double> const& in);

private:
    unsigned int factor_;
    unsigned int samples_;
    fir filter_;
};

} // namespace
