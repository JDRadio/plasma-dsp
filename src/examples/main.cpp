#include "plasma/agc.hpp"
#include "plasma/squelch.hpp"
#include "plasma/qpsk.hpp"
#include "plasma/nco.hpp"
#include "plasma/fir.hpp"
#include "plasma/fir_designer.hpp"
#include "plasma/chirp.hpp"

#include <vector>
#include <string>
#include <iostream>

int main(int argc, char** argv)
{
    plasma::fir_cff fir;
    plasma::fir_decimator_cff fir_decimator = plasma::fir_designer_cff::create_rrc_decimator(2, 6, 0.6);
    plasma::fir_interpolator_cff fir_interpolator = plasma::fir_designer_cff::create_rrc_interpolator(2, 6, 0.6);
    plasma::chirp_f chirp;

    plasma::agc_cf agc;
    agc.set_scale(0.03);
    agc.set_bandwidth(0.003);

    plasma::squelch_cf squelch;
    squelch.set_bandwidth(0.1);
    squelch.set_threshold(-50);
    squelch.set_timeout(240);

    plasma::qpsk_f modem;

    std::string msg = "Hello, world!";
    std::vector<std::complex<float>> out = modem.modulate(msg);

    plasma::nco_f nco;

    nco.set_frequency(0.2);
    nco.set_phase(0);

    auto sent = nco.mix_up(out);

    nco.set_frequency(0.2);
    nco.set_phase(0);

    auto received = nco.mix_down(sent);

    std::string recovered = modem.demodulate(received);

    std::cout << recovered << std::endl;

    return 0;
}
