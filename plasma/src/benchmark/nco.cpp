////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#include <benchmark/benchmark.h>
#include <plasma/nco.hpp>

template <typename SampleType>
void nco_use_lookup(benchmark::State& state)
{
    plasma::nco<SampleType, plasma::use_lookup> nco;
    nco.set_frequency(0.01);

    std::size_t len = state.range(0);
    std::vector<SampleType> vec(len * 2, 0.001);

    for (auto _ : state) {
        nco.mix_up(vec);
    }
}

template <typename SampleType>
void nco_no_lookup(benchmark::State& state)
{
    plasma::nco<SampleType, plasma::no_lookup> nco;
    nco.set_frequency(0.01);

    std::size_t len = state.range(0);
    std::vector<SampleType> vec(len * 2, 0.001);

    for (auto _ : state) {
        nco.mix_down(vec);
    }
}

template <typename SampleType>
void nco_use_lookup_complex(benchmark::State& state)
{
    plasma::nco<SampleType, plasma::use_lookup> nco;
    nco.set_frequency(0.01);

    std::size_t len = state.range(0);
    std::vector<std::complex<SampleType>> vec(len, std::complex<SampleType>{0.001, 0.001});

    for (auto _ : state) {
        nco.mix_up(vec);
    }
}

template <typename SampleType>
void nco_no_lookup_complex(benchmark::State& state)
{
    plasma::nco<SampleType, plasma::no_lookup> nco;
    nco.set_frequency(0.01);

    std::size_t len = state.range(0);
    std::vector<std::complex<SampleType>> vec(len, std::complex<SampleType>{0.001, 0.001});

    for (auto _ : state) {
        nco.mix_down(vec);
    }
}


BENCHMARK_TEMPLATE(nco_use_lookup, float)->Arg(1024)->Arg(4096)->Arg(32768)->Arg(262144)->Arg(1024000)->Arg(4096000)->Arg(16'000'000)->Arg(27'000'000)->Arg(35'000'000)->Arg(50'000'000)->Arg(100'000'000);
BENCHMARK_TEMPLATE(nco_no_lookup, float)->Arg(1024)->Arg(4096)->Arg(32768)->Arg(262144)->Arg(1024000)->Arg(4096000)->Arg(16'000'000)->Arg(27'000'000)->Arg(35'000'000)->Arg(50'000'000)->Arg(100'000'000);
BENCHMARK_TEMPLATE(nco_use_lookup_complex, float)->Arg(1024)->Arg(4096)->Arg(32768)->Arg(262144)->Arg(1024000)->Arg(4096000)->Arg(16'000'000)->Arg(27'000'000)->Arg(35'000'000)->Arg(50'000'000)->Arg(100'000'000);
BENCHMARK_TEMPLATE(nco_no_lookup_complex, float)->Arg(1024)->Arg(4096)->Arg(32768)->Arg(262144)->Arg(1024000)->Arg(4096000)->Arg(16'000'000)->Arg(27'000'000)->Arg(35'000'000)->Arg(50'000'000)->Arg(100'000'000);

BENCHMARK_TEMPLATE(nco_use_lookup, double)->Arg(1024)->Arg(4096)->Arg(32768)->Arg(262144)->Arg(1024000)->Arg(4096000)->Arg(16'000'000)->Arg(27'000'000)->Arg(35'000'000)->Arg(50'000'000)->Arg(100'000'000);
BENCHMARK_TEMPLATE(nco_no_lookup, double)->Arg(1024)->Arg(4096)->Arg(32768)->Arg(262144)->Arg(1024000)->Arg(4096000)->Arg(16'000'000)->Arg(27'000'000)->Arg(35'000'000)->Arg(50'000'000)->Arg(100'000'000);
BENCHMARK_TEMPLATE(nco_use_lookup_complex, double)->Arg(1024)->Arg(4096)->Arg(32768)->Arg(262144)->Arg(1024000)->Arg(4096000)->Arg(16'000'000)->Arg(27'000'000)->Arg(35'000'000)->Arg(50'000'000)->Arg(100'000'000);
BENCHMARK_TEMPLATE(nco_no_lookup_complex, double)->Arg(1024)->Arg(4096)->Arg(32768)->Arg(262144)->Arg(1024000)->Arg(4096000)->Arg(16'000'000)->Arg(27'000'000)->Arg(35'000'000)->Arg(50'000'000)->Arg(100'000'000);
