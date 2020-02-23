#pragma once

#include <complex>
#include <vector>

using namespace std;

namespace dsp {

class agc {
public:
    agc(void);
    ~agc(void) = default;

    double get_max_gain(void) const;
    void set_max_gain(double max_gain);

    double get_min_gain(void) const;
    void set_min_gain(double min_gain);

    void set_gain_limits(double min_gain, double max_gain);

    double get_scale(void) const;
    void set_scale(double scale);

    double get_bandwidth(void) const;
    void set_bandwidth(double bandwidth);

    double get_rssi(void) const;
    void set_rssi(double rssi);

    double get_gain(void) const;
    void set_gain(double gain);

    void lock(void);
    void unlock(void);
    bool locked(void) const;

    double execute(double x);
    vector<double> execute(vector<double> const& in);

    complex<double> execute_complex(complex<double> x);
    vector<complex<double>> execute_complex(vector<complex<double>> const& in);

private:
    void internal_execute(double energy);
    void clamp_gain(void);

    double max_gain_;   //!< maximum gain
    double min_gain_;   //!< minimum gain
    double gain_;       //!< current gain
    double scale_;      //!< output scale
    double alpha_;      //!< feedback gain
    double energy_;     //!< signal energy estimate
    bool locked_;       //!< locked
};

} // namespace
