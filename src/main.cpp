#include "AudioFrontend.hpp"

#include "plasma/dsp/squelch/squelch.hpp"
#include "plasma/dsp/agc/agc.hpp"
#include "plasma/dsp/fir.hpp"
#include "plasma/dsp/math.hpp"

#include <cstdint>
#include <cstddef>
#include <complex>
#include <array>
#include <iostream>
#include <vector>
#include <cmath>

#include <liquid/liquid.h>

constexpr float pi = acos(-1.f);

using namespace std;

unsigned int kaiser_order(float att, float df)
{
    unsigned int n;

    if (att >= 21) {
        n = ceil((att - 7.95f) / (2.285f * (2.f * pi * df)));
    }
    else {
        n = ceil(5.79f / (2.f * pi * df));
    }

    if (n % 2 == 0) {
        return n + 1;
    }

    return n;
}

float kaiser_betaf(float att)
{
    att = abs(att);

    if (att > 50.f) {
        return 0.1102f * (att - 8.7f);
    }
    else if (att < 21.f) {
        return 0.f;
    }
    else {
        return 0.5842f * pow(att - 21.f, 0.4f) + 0.07886f * (att - 21.f);
    }
}

double kaiser_beta(double att)
{
    att = abs(att);

    if (att > 50.) {
        return 0.1102 * (att - 8.7);
    }
    else if (att < 21.) {
        return 0.;
    }
    else {
        return 0.5842 * pow(att - 21., 0.4) + 0.07886 * (att - 21.);
    }
}

float kaiserf(float beta, int n, int big_n)
{
    float t = static_cast<float>(n) - static_cast<float>(big_n - 1) / 2.f;
    float r = 2.f * t / static_cast<float>(big_n - 1);
    float a = cyl_bessel_if(0, beta * sqrt(1.f - r * r));
    float b = cyl_bessel_if(0, beta);

    return a / b;
}

double kaiser(double beta, int n, int big_n)
{
    double t = static_cast<double>(n) - static_cast<double>(big_n - 1) / 2.;
    double r = 2. * t / static_cast<double>(big_n - 1);
    double a = cyl_bessel_i(0, beta * sqrt(1. - r * r));
    double b = cyl_bessel_i(0, beta);

    return a / b;
}

int main(int argc, char* argv[])
{
    AudioFrontend::sptr audio = AudioFrontend::make();

    array<complex<float>, 2048> buffer_in;
    array<complex<float>, 512> buffer_down;
    array<complex<float>, 2048> buffer_up;

    firinterp_cccf resamp_up = firinterp_cccf_create_kaiser(4, 2*8, 40.f);

    double fc = 0.2;
    double att = 80.;
    const unsigned int h_len = kaiser_order(att, 0.01);
    double hf[h_len];
    double beta = kaiser_beta(att);
    for (size_t i = 0; i < h_len; i++) {
        double t = i - (h_len - 1) / 2.;
        hf[i] = 2. * fc * dsp::math::sinc(2. * fc * t);
        hf[i] *= kaiser(beta, i, h_len);
    }
    // liquid_firdes_kaiser(h_len, 0.2f, 40.f, 0.f, hf);

    cout.precision(17);
    for (size_t i = 0; i < h_len; i++) {
        cout << hf[i] << endl;
    }

    dsp::agc agc;
    agc.set_scale(0.01f);
    agc.set_bandwidth(0.003f);

    dsp::squelch squelch;
    squelch.set_bandwidth(0.1f);
    squelch.set_threshold(-50.f);
    squelch.set_timeout(240);

    dsp::fir fir;
    fir.set_taps(vector<double>(hf, hf + h_len));

    while (true) {
        audio->receive(buffer_in.data(), buffer_in.size());
        // 48 kHz
        for (size_t i = 0; i < buffer_in.size(); i++) {
            fir.push_complex(buffer_in[i]);

            if (i % 4 == 0) {
                buffer_down[i/4] = fir.execute_complex();
            }
        }

        // 12 kHz

        for (auto& n : buffer_down) {
            dsp::squelch::status sq = squelch.execute_complex(n);
            n = agc.execute_complex(n);

            if (sq == dsp::squelch::status::on) {
                agc.lock();
                n *= 0;
            }
            else if (sq == dsp::squelch::status::off) {
                agc.unlock();
            }
        }

        // 12 kHz
        firinterp_cccf_execute_block(resamp_up, buffer_down.data(), buffer_down.size(), buffer_up.data());
        // 48 kHz
        audio->transmit(buffer_up.data(), buffer_up.size());
    }

    firinterp_cccf_destroy(resamp_up);

    return 0;
}
