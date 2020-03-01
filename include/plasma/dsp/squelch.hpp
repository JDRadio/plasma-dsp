#pragma once

#include <vector>
#include <complex>

using namespace std;

namespace dsp {

class squelch {
public:
    enum class status {
        off,        //!< no squelch, signal is strong
        on,         //!< squelched, signal is weak
        holding     //!< holding, signal is falling
    };

    squelch(void);
    ~squelch(void) = default;

    void set_status(status s);
    status get_status(void) const;
    bool squelched(void) const;

    double get_threshold(void) const;
    void set_threshold(double threshold);

    unsigned int get_timeout(void) const;
    void set_timeout(unsigned int timeout);

    double get_bandwidth(void) const;
    void set_bandwidth(double bandwidth);

    double get_rssi(void) const;
    void set_rssi(double rssi);

    void lock(void);
    void unlock(void);
    bool locked(void) const;

    status execute(complex<double> x);
    vector<status> execute(vector<complex<double>> const& in);

private:
    status internal_execute(double energy);

    double alpha_;          //!< feedback gain for lowpass filter
    double energy_;         //!< signal energy estimate
    bool locked_;           //!< locked
    double threshold_;      //!< energy threshold in dB
    unsigned int timeout_;  //!< number of samples below threshold before turning on
    unsigned int timer_;    //!< timer in samples below threshold
    status status_;         //!< current status
};

} // namespace
