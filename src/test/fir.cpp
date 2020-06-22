////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the AmateurRadio.Engineer project
//! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
//! \date 2020
//! \copyright AmateurRadio.Engineer
////////////////////////////////////////////////////////////////////////////////
#include <benchmark/benchmark.h>

#include "plasma/fir.hpp"
#include "plasma/fir_designer.hpp"
#include <iostream>

static bool message_displayed = true;

template <typename T, typename T_TAPS>
void fir_push(benchmark::State& state)
{
    plasma::FIR<T,T_TAPS> fir = plasma::FIR_DESIGNER<T,T_TAPS>::create_kaiser_filter(0.25, 0.01, 60.0);
    T x = 0.001;

    if (! message_displayed) {
        message_displayed = true;
        std::cout << "Filter taps: " << fir.get_delay() << std::endl;
    }

    for (auto _ : state) {
        fir.push(x);
    }
}

template <typename T, typename T_TAPS>
void fir_execute_one(benchmark::State& state)
{
    plasma::FIR<T,T_TAPS> fir = plasma::FIR_DESIGNER<T,T_TAPS>::create_kaiser_filter(0.25, 0.01, 60.0);
    T x = 0.001;

    for (std::size_t i = 0; i < fir.get_delay() * 2 + 1; ++i) {
        fir.push(x);
    }

    for (auto _ : state) {
        x = fir.execute();
    }
}

template <typename T, typename T_TAPS>
void fir_execute_vector(benchmark::State& state)
{
    plasma::FIR<T,T_TAPS> fir = plasma::FIR_DESIGNER<T,T_TAPS>::create_kaiser_filter(0.25, 0.01, 60.0);
    std::vector<T> vec(state.range(0));
    std::fill_n(vec.begin(), vec.size(), T(1));
    std::vector<T> out;

    for (std::size_t i = 0; i < fir.get_delay() * 2 + 1; ++i) {
        fir.push(T(1));
    }

    for (auto _ : state) {
        out = fir.execute(vec);
    }
}

// fir_ff
BENCHMARK_TEMPLATE(fir_push, float, float);
BENCHMARK_TEMPLATE(fir_execute_one, float, float);
BENCHMARK_TEMPLATE(fir_execute_vector, float, float)->Arg(1024)->Arg(4096);


// fir_dd
BENCHMARK_TEMPLATE(fir_push, double, double);
BENCHMARK_TEMPLATE(fir_execute_one, double, double);
BENCHMARK_TEMPLATE(fir_execute_vector, double, double)->Arg(1024)->Arg(4096);


// fir_cff
BENCHMARK_TEMPLATE(fir_push, std::complex<float>, float);
BENCHMARK_TEMPLATE(fir_execute_one, std::complex<float>, float);
BENCHMARK_TEMPLATE(fir_execute_vector, std::complex<float>, float)->Arg(1024)->Arg(4096);


// fir_cdd
BENCHMARK_TEMPLATE(fir_push, std::complex<double>, double);
BENCHMARK_TEMPLATE(fir_execute_one, std::complex<double>, double);
BENCHMARK_TEMPLATE(fir_execute_vector, std::complex<double>, double)->Arg(1024)->Arg(4096);


// fir_cfcf
BENCHMARK_TEMPLATE(fir_push, std::complex<float>, std::complex<float>);
BENCHMARK_TEMPLATE(fir_execute_one, std::complex<float>, std::complex<float>);
BENCHMARK_TEMPLATE(fir_execute_vector, std::complex<float>, std::complex<float>)->Arg(1024)->Arg(4096);


// fir_cdcd
BENCHMARK_TEMPLATE(fir_push, std::complex<double>, std::complex<double>);
BENCHMARK_TEMPLATE(fir_execute_one, std::complex<double>, std::complex<double>);
BENCHMARK_TEMPLATE(fir_execute_vector, std::complex<double>, std::complex<double>)->Arg(1024)->Arg(4096);
