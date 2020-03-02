#include "AudioFrontend.hpp"

#include "plasma/dsp/squelch.hpp"
#include "plasma/dsp/agc.hpp"
#include "plasma/dsp/nco.hpp"
#include "plasma/dsp/chirp.hpp"
#include "plasma/dsp/fir.hpp"
#include "plasma/dsp/math.hpp"
#include "plasma/dsp/qpsk.hpp"
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
#include <fstream>
#include <sstream>

using namespace std;

void print_to_file(string const& file, vector<complex<double>> const& in)
{
    ofstream fp(file);
    for (auto val : in) {
        fp << val.real() << " " << val.imag() << endl;
    }
}

void print_to_file(string const& file, vector<double> const& in)
{
    ofstream fp(file);
    for (auto val : in) {
        fp << val << endl;
    }
}

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

    double fc = 0.2;
    double df = 0.01;
    double att = 80;
    dsp::fir fir = dsp::fir_designer::create_kaiser_filter(fc, df, att);

    dsp::fir_decimator decimator = dsp::fir_designer::create_kaiser_decimator(2, 0.05, 60);

    dsp::fir_interpolator interpolator = dsp::fir_designer::create_kaiser_interpolator(2, 0.05, 60);

    dsp::fir_interpolator rrc_tx = dsp::fir_designer::create_rrc_interpolator(2, 6, 0.6);
    dsp::fir_decimator rrc_rx = dsp::fir_designer::create_rrc_decimator(2, 6, 0.6);

    dsp::qpsk qpsk;

    {
        string message_tx = "Hello, World! Hello, World!"s;

        vector<complex<double>> sig_tx_mod = qpsk.modulate(message_tx);
        vector<complex<double>> sig_tx_rrc = rrc_tx.execute(sig_tx_mod);
        vector<complex<double>> sig_tx_interp = interpolator.execute(sig_tx_rrc);
        vector<complex<double>> sig_rx_decim = decimator.execute(sig_tx_interp);
        vector<complex<double>> sig_rx_rrc = rrc_rx.execute(sig_rx_decim);
        string message_rx = qpsk.demodulate(sig_rx_rrc);

        // print_to_file("sig_tx_mod.raw", sig_tx_mod);
        // print_to_file("sig_tx_rrc.raw", sig_tx_rrc);
        // print_to_file("sig_tx_interp.raw", sig_tx_interp);
        // print_to_file("sig_rx_decim.raw", sig_rx_decim);
        // print_to_file("sig_rx_rrc.raw", sig_rx_rrc);
    }

    return 0;
}
