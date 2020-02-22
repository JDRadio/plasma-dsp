#pragma once

#include <type_traits>
#include <complex>
#include <vector>

using namespace std;

namespace dsp {

namespace _ {
template <class T, class Enable = void>
class agc{};
}

template <class T>
class agc_t : private _::agc<T, typename enable_if<is_floating_point_v<T>>::type> {
public:
    agc_t(void);
    ~agc_t(void) = default;

    T get_max_gain(void) const;
    void set_max_gain(T max_gain);

    T get_min_gain(void) const;
    void set_min_gain(T min_gain);

    void set_gain_limits(T min_gain, T max_gain);

    T get_scale(void) const;
    void set_scale(T scale);

    T get_bandwidth(void) const;
    void set_bandwidth(T bandwidth);

    T get_rssi(void) const;
    void set_rssi(T rssi);

    T get_gain(void) const;
    void set_gain(T gain);

    void lock(void);
    void unlock(void);
    bool locked(void) const;

    T execute(T x);
    vector<T> execute(vector<T> const& in);

    complex<T> execute_complex(complex<T> x);
    vector<complex<T>> execute_complex(vector<complex<T>> const& in);

private:
    void internal_execute(T energy);
    void clamp_gain(void);

    T max_gain_;    //!< maximum gain
    T min_gain_;    //!< minimum gain
    T gain_;        //!< current gain
    T scale_;       //!< output scale
    T alpha_;       //!< feedback gain
    T energy_;      //!< signal energy estimate
    bool locked_;   //!< locked
};

typedef agc_t<double> agc;
typedef agc_t<float> agcf;

} // namespace

#include "src/agc.tci"
