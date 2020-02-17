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

#include <liquid/liquid.h>

using namespace std;

int main(int argc, char* argv[])
{
    AudioFrontend::sptr audio = AudioFrontend::make();

    array<complex<float>, 2048> buffer_in;
    array<complex<float>, 512> buffer_down;
    array<complex<float>, 2048> buffer_up;

    firdecim_cccf resamp_down = firdecim_cccf_create_kaiser(4, 2*8, 40.f);
    firinterp_cccf resamp_up = firinterp_cccf_create_kaiser(4, 2*8, 40.f);

    dsp::agc agc;
    agc.set_scale(0.01f);
    agc.set_bandwidth(0.003f);

    dsp::squelch squelch;
    squelch.set_bandwidth(0.1f);
    squelch.set_threshold(-50.f);
    squelch.set_timeout(240);

    dsp::fir fir;
    fir.set_taps({1.f/3.f, 1.f/3.f, 1.f/3.f});

    while (true) {
        audio->receive(buffer_in.data(), buffer_in.size());
        // 48 kHz
        firdecim_cccf_execute_block(resamp_down, buffer_in.data(), buffer_down.size(), buffer_down.data());
        // 12 kHz

        for (auto& n : buffer_down) {
            dsp::squelch::status sq = squelch.execute(n);
            n = agc.execute(n);

            fir.push_complex(n);
            n = fir.execute_complex();

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

    firdecim_cccf_destroy(resamp_down);
    firinterp_cccf_destroy(resamp_up);

    return 0;
}
