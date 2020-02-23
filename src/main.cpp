#include "AudioFrontend.hpp"

#include "plasma/dsp/squelch.hpp"
#include "plasma/dsp/agc.hpp"
#include "plasma/dsp/fir.hpp"
#include "plasma/dsp/math.hpp"
#include "plasma/dsp/fir_designer.hpp"

#include <cstdint>
#include <cstddef>
#include <complex>
#include <array>
#include <iostream>
#include <vector>
#include <cmath>

#include <liquid/liquid.h>

using namespace std;

int main(int, char*[])
{
    AudioFrontend::sptr audio = AudioFrontend::make();

    array<complex<float>, 2048> buffer_in;
    array<complex<float>, 512> buffer_down;
    array<complex<float>, 2048> buffer_up;

    firinterp_cccf resamp_up = firinterp_cccf_create_kaiser(4, 2*8, 40);

    dsp::agc agc;
    agc.set_scale(0.01);
    agc.set_bandwidth(0.003);

    dsp::squelch squelch;
    squelch.set_bandwidth(0.1);
    squelch.set_threshold(-50);
    squelch.set_timeout(240);

    dsp::fir fir;
    double fc = 0.2;
    double df = 0.01;
    double att = 80;
    fir.set_taps(dsp::fir_designer::create_kaiser(fc, df, att));

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
                n = 0;
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
