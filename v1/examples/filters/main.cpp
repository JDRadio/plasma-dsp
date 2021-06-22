////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#include <plasma/filters/windowed.hpp>
#include <plasma/windows/kaiser.hpp>
#include <plasma/filter/filter.hpp>
#include <plasma/noise/wgn.hpp>
#include <fstream>
#include <iomanip>
#include <fmt/format.h>

int main(int, char**)
{
    // Define filter specifications
    static constexpr double CUTOFF_FREQUENCY = 0.15;
    static constexpr double TRANSITION_BANDWIDTH = 0.2;
    static constexpr double ATTENUATION = 60.0;

    // Create a Kaiser filter
    plasma::windows::kaiser kaiserWindow{plasma::windows::kaiser::get_beta(ATTENUATION)};
    auto kaiserTaps = plasma::filters::windowed::create_taps_single(CUTOFF_FREQUENCY, TRANSITION_BANDWIDTH, ATTENUATION, kaiserWindow);
    plasma::filter kaiserFilter{kaiserTaps};

    // Generate white gaussian noise
    static constexpr double NOISE_POWER = 0;
    static constexpr unsigned int NUM_NOISE_SAMPLES = 1024 * 2;
    auto noise = plasma::noise::wgn::create_single(NOISE_POWER, NUM_NOISE_SAMPLES);

    // Filter the noise
    auto output = kaiserFilter.execute(noise);

    // Output taps
    {
        std::ofstream fp{"taps.dat", std::ofstream::binary};
        auto const size = kaiserTaps.size();
        fp << std::setprecision(9);
        for (unsigned int n = 0; n < size; ++n) {
            fp << kaiserTaps[n] << std::endl;
        }
    }

    // Output noise
    {
        std::ofstream fp{"in.dat", std::ofstream::binary};
        auto const size = noise.size();
        fp << std::setprecision(9);
        for (unsigned int n = 0; n < size; n += 2) {
            fp << noise[n] << " " << noise[n + 1] << std::endl;
        }
    }

    // Output results
    {
        std::ofstream fp{"out.dat", std::ofstream::binary};
        auto const size = output.size();
        fp << std::setprecision(9);
        for (unsigned int n = 0; n < size; n += 2) {
            fp << output[n] << " " << output[n + 1] << std::endl;
        }
    }

    return 0;
}
