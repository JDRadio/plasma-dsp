#pragma once

#include <vector>

using namespace std;

namespace plasma::audio {

class fir {
public:
    fir(void) = default;
    ~fir(void) = default;

    void set_taps(vector<double> const& taps);

    unsigned int get_delay(void) const;

    void reset(void);

    void push(double x);
    double execute(void);

    vector<double> execute(vector<double> const& in);

private:
    vector<double> taps_;
    vector<double> samples_;
};

} // namespace
