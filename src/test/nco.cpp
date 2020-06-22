////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the AmateurRadio.Engineer project
//! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
//! \date 2020
//! \copyright AmateurRadio.Engineer
////////////////////////////////////////////////////////////////////////////////
#include <benchmark/benchmark.h>

#include "plasma/nco.hpp"

template <typename T>
void nco_constructor(benchmark::State& state)
{
    for (auto _ : state) {
        plasma::NCO<T> nco;
    }
}

template <typename T>
void nco_get_angular_frequency(benchmark::State& state)
{
    plasma::NCO<T> nco;
    double x = 0;

    for (auto _ : state) {
        x = nco.get_angular_frequency();
    }
}

template <typename T>
void nco_set_angular_frequency(benchmark::State& state)
{
    plasma::NCO<T> nco;
    double x = 0.000001;

    for (auto _ : state) {
        nco.set_angular_frequency(x);
    }
}

template <typename T>
void nco_adjust_angular_frequency(benchmark::State& state)
{
    plasma::NCO<T> nco;
    double x = 0.000001;

    for (auto _ : state) {
        nco.adjust_angular_frequency(x);
    }
}

template <typename T>
void nco_get_frequency(benchmark::State& state)
{
    plasma::NCO<T> nco;
    double x = 0;

    for (auto _ : state) {
        x = nco.get_frequency();
    }
}

template <typename T>
void nco_set_frequency(benchmark::State& state)
{
    plasma::NCO<T> nco;
    double x = 0.000001;

    for (auto _ : state) {
        nco.set_frequency(x);
    }
}

template <typename T>
void nco_adjust_frequency(benchmark::State& state)
{
    plasma::NCO<T> nco;
    double x = 0.000001;

    for (auto _ : state) {
        nco.adjust_frequency(x);
    }
}

template <typename T>
void nco_get_phase(benchmark::State& state)
{
    plasma::NCO<T> nco;
    double x = 0.000001;

    for (auto _ : state) {
        x = nco.get_phase();
    }
}

template <typename T>
void nco_set_phase(benchmark::State& state)
{
    plasma::NCO<T> nco;
    double x = 0.000001;

    for (auto _ : state) {
        nco.set_phase(x);
    }
}

template <typename T>
void nco_adjust_phase(benchmark::State& state)
{
    plasma::NCO<T> nco;
    double x = 0.000001;

    for (auto _ : state) {
        nco.adjust_phase(x);
    }
}

template <typename T>
void nco_set_pll_bandwidth(benchmark::State& state)
{
    plasma::NCO<T> nco;
    double x = 0.000001;

    for (auto _ : state) {
        nco.set_pll_bandwidth(x);
    }
}

template <typename T>
void nco_pll_step(benchmark::State& state)
{
    plasma::NCO<T> nco;
    double x = 0.000001;

    for (auto _ : state) {
        nco.pll_step(x);
    }
}

template <typename T>
void nco_step(benchmark::State& state)
{
    plasma::NCO<T> nco;
    double x = 0.000001;
    nco.set_frequency(x);

    for (auto _ : state) {
        nco.step();
    }
}

template <typename T>
void nco_reset(benchmark::State& state)
{
    plasma::NCO<T> nco;

    for (auto _ : state) {
        nco.reset();
    }
}

template <typename T>
void nco_mix_up_one(benchmark::State& state)
{
    plasma::NCO<T> nco;
    std::complex<float> x(0.001, 0.001);
    nco.set_frequency(0.000001);

    for (auto _ : state) {
        x = nco.mix_up(x);
    }
}

template <typename T>
void nco_mix_up_vector(benchmark::State& state)
{
    plasma::NCO<T> nco;
    nco.set_frequency(0.000001);
    std::vector<std::complex<T>> vec(state.range(0));
    std::fill_n(vec.begin(), vec.size(), std::complex<T>(0.001, 0.001));
    std::vector<std::complex<T>> out;

    for (auto _ : state) {
        out = nco.mix_up(vec);
    }
}

template <typename T>
void nco_mix_down_one(benchmark::State& state)
{
    plasma::NCO<T> nco;
    std::complex<T> x(0.001, 0.001);
    nco.set_frequency(0.000001);

    for (auto _ : state) {
        x = nco.mix_down(x);
    }
}

template <typename T>
void nco_mix_down_vector(benchmark::State& state)
{
    plasma::NCO<T> nco;
    nco.set_frequency(0.000001);
    std::vector<std::complex<T>> vec(state.range(0));
    std::fill_n(vec.begin(), vec.size(), std::complex<T>(0.001, 0.001));
    std::vector<std::complex<T>> out;

    for (auto _ : state) {
        out = nco.mix_down(vec);
    }
}

template <typename T>
void nco_typical_usage(benchmark::State& state)
{
    plasma::NCO<T> nco;
    std::complex<T> x(0.001, 0.001);
    std::size_t len = state.range(0);

    for (auto _ : state) {
        for (std::size_t i = 0; i < len; ++i) {
            x = nco.mix_down(x);
            nco.step();
        }
    }
}

BENCHMARK_TEMPLATE(nco_constructor, float);
BENCHMARK_TEMPLATE(nco_constructor, double);
BENCHMARK_TEMPLATE(nco_get_angular_frequency, float);
BENCHMARK_TEMPLATE(nco_get_angular_frequency, double);
BENCHMARK_TEMPLATE(nco_set_angular_frequency, float);
BENCHMARK_TEMPLATE(nco_set_angular_frequency, double);
BENCHMARK_TEMPLATE(nco_adjust_angular_frequency, float);
BENCHMARK_TEMPLATE(nco_adjust_angular_frequency, double);
BENCHMARK_TEMPLATE(nco_get_frequency, float);
BENCHMARK_TEMPLATE(nco_get_frequency, double);
BENCHMARK_TEMPLATE(nco_set_frequency, float);
BENCHMARK_TEMPLATE(nco_set_frequency, double);
BENCHMARK_TEMPLATE(nco_adjust_frequency, float);
BENCHMARK_TEMPLATE(nco_adjust_frequency, double);
BENCHMARK_TEMPLATE(nco_get_phase, float);
BENCHMARK_TEMPLATE(nco_get_phase, double);
BENCHMARK_TEMPLATE(nco_set_phase, float);
BENCHMARK_TEMPLATE(nco_set_phase, double);
BENCHMARK_TEMPLATE(nco_adjust_phase, float);
BENCHMARK_TEMPLATE(nco_adjust_phase, double);
BENCHMARK_TEMPLATE(nco_set_pll_bandwidth, float);
BENCHMARK_TEMPLATE(nco_set_pll_bandwidth, double);
BENCHMARK_TEMPLATE(nco_pll_step, float);
BENCHMARK_TEMPLATE(nco_pll_step, double);
BENCHMARK_TEMPLATE(nco_step, float);
BENCHMARK_TEMPLATE(nco_step, double);
BENCHMARK_TEMPLATE(nco_reset, float);
BENCHMARK_TEMPLATE(nco_reset, double);
BENCHMARK_TEMPLATE(nco_mix_up_one, float);
BENCHMARK_TEMPLATE(nco_mix_up_one, double);
BENCHMARK_TEMPLATE(nco_mix_up_vector, float)->Arg(1024)->Arg(4096);
BENCHMARK_TEMPLATE(nco_mix_up_vector, double)->Arg(1024)->Arg(4096);
BENCHMARK_TEMPLATE(nco_mix_down_one, float);
BENCHMARK_TEMPLATE(nco_mix_down_one, double);
BENCHMARK_TEMPLATE(nco_mix_down_vector, float)->Arg(1024)->Arg(4096);
BENCHMARK_TEMPLATE(nco_mix_down_vector, double)->Arg(1024)->Arg(4096);
BENCHMARK_TEMPLATE(nco_typical_usage, float)->Arg(4096);
BENCHMARK_TEMPLATE(nco_typical_usage, double)->Arg(4096);
