#include "AudioFrontend.hpp"

#include "dsp/squelch.hpp"
#include "dsp/agc.hpp"
#include "dsp/fir.hpp"

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

float sinc(float x)
{
    if (abs(x) < 0.01f) {
        return cos(pi * x / 2.f) * cos(pi * x / 4.f) * cos(pi * x / 6.f);
    }

    return sin(pi * x) / (pi * x);
}

unsigned int kaiser_n(float att, float df)
{
    unsigned int n;

    if (att >= 21) {
        n = ceil((att - 7.95f) / (2.285f * (2.f * df)));
    }
    else {
        n = ceil(5.79f / (2.f * df));
    }

    if (n % 2 == 0) {
        return n;
    }

    return n + 1;
}

float kaiser_b(float att)
{
    att = abs(att);

    if (att > 50.f) {
        return 0.1102f * (att - 8.7f);
    }
    else if (att < 21.f) {
        return 0.f;
    }
    else {
        return 0.5842f * pow(att - 21.f, 0.4f) + 0.07886 * (att - 21.f);
    }
}

int main(int argc, char* argv[])
{
    AudioFrontend::sptr audio = AudioFrontend::make();

    array<complex<float>, 2048> buffer_in;
    array<complex<float>, 512> buffer_down;
    array<complex<float>, 2048> buffer_up;

    firinterp_cccf resamp_up = firinterp_cccf_create_kaiser(4, 2*8, 40.f);

    const unsigned int h_len = 2*(4)*(2*8)+1;
    float hf[h_len];
    for (size_t i = 0; i < h_len; i++) {
        float t = i - (h_len - 1.f) / 2.f;
        hf[i] = sinc(2.f * 0.5f * t);
    }
    //liquid_firdes_kaiser(h_len, 0.5f, 40.f, 0.f, hf);

    dsp::agc agc;
    agc.set_scale(0.01f);
    agc.set_bandwidth(0.003f);

    dsp::squelch squelch;
    squelch.set_bandwidth(0.1f);
    squelch.set_threshold(-50.f);
    squelch.set_timeout(240);

    dsp::fir fir;
    fir.set_taps(vector<float>(hf, hf + h_len));

    for (size_t i = 0; i < h_len; i++) {
        cout << hf[i] << endl;
    }

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
            dsp::squelch::status sq = squelch.execute(n);
            n = agc.execute(n);

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
