////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_DSP_NOISE_WGN_HPP_
#define PLASMA_DSP_NOISE_WGN_HPP_

#include <vector>
#include <random>

namespace plasma::noise {

class wgn
{
    wgn(void) noexcept = delete;
    ~wgn(void) noexcept = delete;

public:
    static std::vector<float> create_single(double power, unsigned int num_samples) noexcept;
    static std::vector<double> create_double(double power, unsigned int num_samples) noexcept;

    template <typename T>
    static std::vector<T> create(double power, unsigned int num_samples) noexcept
    {
        const T mean = 0;
        const T stddev = std::pow(10.0, -power / 20.0);

        std::normal_distribution<T> dist{mean, stddev};
        std::default_random_engine generator{std::random_device{}()};

        std::vector<T> out(num_samples);

        for (unsigned int n = 0; n < num_samples; ++n) {
            out[n] = dist(generator);
        }

        return out;
    }
};

} // namespace

#endif
