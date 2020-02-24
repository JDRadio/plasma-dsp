#include "AudioFrontend.hpp"

#include "plasma/dsp/squelch.hpp"
#include "plasma/dsp/agc.hpp"
#include "plasma/dsp/fir.hpp"
#include "plasma/dsp/math.hpp"
#include "plasma/dsp/fir_designer.hpp"
#include "plasma/dsp/fir_decimator.hpp"
#include "plasma/dsp/fir_interpolator.hpp"

#include <cstdint>
#include <cstddef>
#include <array>
#include <iostream>
#include <vector>
#include <complex>
#include <cmath>


using namespace std;

int main(int, char*[])
{
    AudioFrontend::sptr audio = AudioFrontend::make();

    array<complex<double>, 2048> buffer_in;

    dsp::agc agc;
    agc.set_scale(0.03);
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

    dsp::fir_decimator decimator;
    decimator.set_factor(2);

    dsp::fir_interpolator interpolator;
    interpolator.set_factor(2);

    while (true) {
        audio->receive(buffer_in.data(), buffer_in.size());

        // 48 kHz
        auto buffer_down = decimator.execute_complex(vector<complex<double>>(buffer_in.data(), buffer_in.data() + buffer_in.size()));
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
        auto buffer_up = interpolator.execute_complex(buffer_down);
        // 48 kHz

        audio->transmit(buffer_up.data(), buffer_up.size());
    }

    return 0;
}
