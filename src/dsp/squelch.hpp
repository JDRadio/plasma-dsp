#pragma once

#include <vector>
#include <complex>

using namespace std;

namespace dsp {

class squelch {
public:
    enum class status {
        off,        // no squelch, signal is strong
        on,         // squelched, signal is weak
        holding     // holding, signal is falling
    };

    squelch(void);
    ~squelch(void) = default;

    void set_status(status s);
    status get_status(void) const;
    bool squelched(void) const;

    float get_threshold(void) const;
    void set_threshold(float threshold);

    unsigned int get_timeout(void) const;
    void set_timeout(unsigned int timeout);

    float get_bandwidth(void) const;
    void set_bandwidth(float bandwidth);

    float get_rssi(void) const;
    void set_rssi(float rssi);

    void lock(void);
    void unlock(void);
    bool locked(void) const;

    status execute(float x);
    vector<status> execute(vector<float> const& in);

    status execute(complex<float> x);
    vector<status> execute(vector<complex<float>> const& in);

private:
    status internal_execute(float energy);

    float alpha_;           //!< feedback gain for lowpass filter
    float energy_;          //!< signal energy estimate
    bool locked_;           //!< locked
    float threshold_;       //!< energy threshold in dB
    unsigned int timeout_;  //!< number of samples below threshold before turning on
    unsigned int timer_;    //!< timer in samples below threshold
    status status_;         //!< current status
};

} // namespace
