#pragma once

#include <complex>
#include <vector>

using namespace std;

namespace dsp {

class agc {
public:
    agc(void);
    ~agc(void) = default;

    float get_max_gain(void) const;
    void set_max_gain(float max_gain);

    float get_min_gain(void) const;
    void set_min_gain(float min_gain);

    void set_gain_limits(float min_gain, float max_gain);

    float get_scale(void) const;
    void set_scale(float scale);

    float get_bandwidth(void) const;
    void set_bandwidth(float bandwidth);

    float get_rssi(void) const;
    void set_rssi(float rssi);

    float get_gain(void) const;
    void set_gain(float gain);

    void lock(void);
    void unlock(void);
    bool locked(void) const;

    float execute(float x);
    vector<float> execute(vector<float> const& in);

    complex<float> execute(complex<float> x);
    vector<complex<float>> execute(vector<complex<float>> const& in);

private:
    void internal_execute(float energy);
    void clamp_gain(void);

    float max_gain_;    //!< maximum gain
    float min_gain_;    //!< minimum gain
    float gain_;        //!< current gain
    float scale_;       //!< output scale
    float alpha_;       //!< feedback gain
    float energy_;      //!< signal energy estimate
    bool locked_;       //!< locked
};

} // namespace
