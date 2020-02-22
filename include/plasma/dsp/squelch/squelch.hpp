#pragma once

#include <vector>
#include <complex>

using namespace std;

namespace dsp {

namespace _ {
template <class T, class Enable = void>
class squelch{};
}

template <class T>
class squelch_t : private _::squelch<T, typename enable_if<is_floating_point_v<T>>::type> {
public:
    enum class status {
        off,        //!< no squelch, signal is strong
        on,         //!< squelched, signal is weak
        holding     //!< holding, signal is falling
    };

    squelch_t(void);
    ~squelch_t(void) = default;

    void set_status(status s);
    status get_status(void) const;
    bool squelched(void) const;

    T get_threshold(void) const;
    void set_threshold(T threshold);

    unsigned int get_timeout(void) const;
    void set_timeout(unsigned int timeout);

    T get_bandwidth(void) const;
    void set_bandwidth(T bandwidth);

    T get_rssi(void) const;
    void set_rssi(T rssi);

    void lock(void);
    void unlock(void);
    bool locked(void) const;

    status execute(T x);
    vector<status> execute(vector<T> const& in);

    status execute_complex(complex<T> x);
    vector<status> execute_complex(vector<complex<T>> const& in);

private:
    status internal_execute(T energy);

    T alpha_;               //!< feedback gain for lowpass filter
    T energy_;              //!< signal energy estimate
    bool locked_;           //!< locked
    T threshold_;           //!< energy threshold in dB
    unsigned int timeout_;  //!< number of samples below threshold before turning on
    unsigned int timer_;    //!< timer in samples below threshold
    status status_;         //!< current status
};

typedef squelch_t<double> squelch;
typedef squelch_t<float> squelchf;

} // namespace

#include "src/squelch.tci"
