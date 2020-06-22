////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the AmateurRadio.Engineer project
//! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
//! \date 2020
//! \copyright AmateurRadio.Engineer
////////////////////////////////////////////////////////////////////////////////
#include <benchmark/benchmark.h>

#include "plasma/agc.hpp"

template <typename T, typename TC>
void agc_constructor(benchmark::State& state)
{
    for (auto _ : state) {
        plasma::AGC<T,TC> agc;
    }
}

template <typename T, typename TC>
void agc_get_max_gain(benchmark::State& state)
{
    plasma::AGC<T,TC> agc;
    T x = 0;

    for (auto _ : state) {
        x = agc.get_max_gain();
    }
}

template <typename T, typename TC>
void agc_set_max_gain(benchmark::State& state)
{
    plasma::AGC<T,TC> agc;
    T x = 0.000001;

    for (auto _ : state) {
        agc.set_max_gain(x);
    }
}

template <typename T, typename TC>
void agc_get_min_gain(benchmark::State& state)
{
    plasma::AGC<T,TC> agc;
    T x = 0.000001;

    for (auto _ : state) {
        agc.get_min_gain();
    }
}

template <typename T, typename TC>
void agc_set_min_gain(benchmark::State& state)
{
    plasma::AGC<T,TC> agc;
    T x = 0.000001;

    for (auto _ : state) {
        agc.set_min_gain(x);
    }
}

template <typename T, typename TC>
void agc_set_gain_limits(benchmark::State& state)
{
    plasma::AGC<T,TC> agc;
    T x = 1e-6;
    T y = 1e6;

    for (auto _ : state) {
        agc.set_gain_limits(x, y);
    }
}

template <typename T, typename TC>
void agc_get_scale(benchmark::State& state)
{
    plasma::AGC<T,TC> agc;
    T x = 0.000001;

    for (auto _ : state) {
        x = agc.get_scale();
    }
}

template <typename T, typename TC>
void agc_set_scale(benchmark::State& state)
{
    plasma::AGC<T,TC> agc;
    T x = 0.000001;

    for (auto _ : state) {
        agc.set_scale(x);
    }
}

template <typename T, typename TC>
void agc_get_bandwidth(benchmark::State& state)
{
    plasma::AGC<T,TC> agc;
    T x = 0.000001;

    for (auto _ : state) {
        x = agc.get_bandwidth();
    }
}

template <typename T, typename TC>
void agc_set_bandwidth(benchmark::State& state)
{
    plasma::AGC<T,TC> agc;
    T x = 0.000001;

    for (auto _ : state) {
        agc.set_bandwidth(x);
    }
}

template <typename T, typename TC>
void agc_get_rssi(benchmark::State& state)
{
    plasma::AGC<T,TC> agc;
    T x = 0.000001;

    for (auto _ : state) {
        x = agc.get_rssi();
    }
}

template <typename T, typename TC>
void agc_set_rssi(benchmark::State& state)
{
    plasma::AGC<T,TC> agc;
    T x = 0.000001;

    for (auto _ : state) {
        agc.set_rssi(x);
    }
}

template <typename T, typename TC>
void agc_get_gain(benchmark::State& state)
{
    plasma::AGC<T,TC> agc;
    T x = 0.000001;

    for (auto _ : state) {
        x = agc.get_gain();
    }
}

template <typename T, typename TC>
void agc_set_gain(benchmark::State& state)
{
    plasma::AGC<T,TC> agc;
    T x = 0.000001;

    for (auto _ : state) {
        agc.set_gain(x);
    }
}

template <typename T, typename TC>
void agc_lock(benchmark::State& state)
{
    plasma::AGC<T,TC> agc;

    for (auto _ : state) {
        agc.lock();
    }
}

template <typename T, typename TC>
void agc_unlock(benchmark::State& state)
{
    plasma::AGC<T,TC> agc;

    for (auto _ : state) {
        agc.unlock();
    }
}

template <typename T, typename TC>
void agc_locked(benchmark::State& state)
{
    plasma::AGC<T,TC> agc;
    bool x = false;

    for (auto _ : state) {
        x = agc.locked();
    }
}

template <typename T, typename TC>
void agc_execute_one(benchmark::State& state)
{
    plasma::AGC<T,TC> agc;
    TC x(1);

    for (auto _ : state) {
        x = agc.execute(x);
    }
}

template <typename T, typename TC>
void agc_execute_vector_inplace(benchmark::State& state)
{
    plasma::AGC<T,TC> agc;
    std::vector<TC> vec(state.range(0));
    std::fill_n(vec.begin(), vec.size(), TC(1));

    for (auto _ : state) {
        agc.execute(vec);
    }
}

template <typename T, typename TC>
void agc_execute_vector(benchmark::State& state)
{
    plasma::AGC<T,TC> agc;
    std::vector<TC> vec(state.range(0));
    std::fill_n(vec.begin(), vec.size(), TC(1));
    std::vector<TC> out;

    for (auto _ : state) {
        agc.execute(vec, out);
    }
}

// agc_f
BENCHMARK_TEMPLATE(agc_constructor, float, float);
BENCHMARK_TEMPLATE(agc_get_max_gain, float, float);
BENCHMARK_TEMPLATE(agc_set_max_gain, float, float);
BENCHMARK_TEMPLATE(agc_get_min_gain, float, float);
BENCHMARK_TEMPLATE(agc_set_min_gain, float, float);
BENCHMARK_TEMPLATE(agc_set_gain_limits, float, float);
BENCHMARK_TEMPLATE(agc_get_scale, float, float);
BENCHMARK_TEMPLATE(agc_set_scale, float, float);
BENCHMARK_TEMPLATE(agc_get_bandwidth, float, float);
BENCHMARK_TEMPLATE(agc_set_bandwidth, float, float);
BENCHMARK_TEMPLATE(agc_get_rssi, float, float);
BENCHMARK_TEMPLATE(agc_set_rssi, float, float);
BENCHMARK_TEMPLATE(agc_get_gain, float, float);
BENCHMARK_TEMPLATE(agc_set_gain, float, float);
BENCHMARK_TEMPLATE(agc_lock, float, float);
BENCHMARK_TEMPLATE(agc_unlock, float, float);
BENCHMARK_TEMPLATE(agc_locked, float, float);
BENCHMARK_TEMPLATE(agc_execute_one, float, float);
BENCHMARK_TEMPLATE(agc_execute_vector_inplace, float, float)->Arg(1024)->Arg(4096);
BENCHMARK_TEMPLATE(agc_execute_vector, float, float)->Arg(1024)->Arg(4096);

// agc_d
BENCHMARK_TEMPLATE(agc_constructor, double, double);
BENCHMARK_TEMPLATE(agc_get_max_gain, double, double);
BENCHMARK_TEMPLATE(agc_set_max_gain, double, double);
BENCHMARK_TEMPLATE(agc_get_min_gain, double, double);
BENCHMARK_TEMPLATE(agc_set_min_gain, double, double);
BENCHMARK_TEMPLATE(agc_set_gain_limits, double, double);
BENCHMARK_TEMPLATE(agc_get_scale, double, double);
BENCHMARK_TEMPLATE(agc_set_scale, double, double);
BENCHMARK_TEMPLATE(agc_get_bandwidth, double, double);
BENCHMARK_TEMPLATE(agc_set_bandwidth, double, double);
BENCHMARK_TEMPLATE(agc_get_rssi, double, double);
BENCHMARK_TEMPLATE(agc_set_rssi, double, double);
BENCHMARK_TEMPLATE(agc_get_gain, double, double);
BENCHMARK_TEMPLATE(agc_set_gain, double, double);
BENCHMARK_TEMPLATE(agc_lock, double, double);
BENCHMARK_TEMPLATE(agc_unlock, double, double);
BENCHMARK_TEMPLATE(agc_locked, double, double);
BENCHMARK_TEMPLATE(agc_execute_one, double, double);
BENCHMARK_TEMPLATE(agc_execute_vector_inplace, double, double)->Arg(1024)->Arg(4096);
BENCHMARK_TEMPLATE(agc_execute_vector, double, double)->Arg(1024)->Arg(4096);

// agc_cf
BENCHMARK_TEMPLATE(agc_constructor, float, std::complex<float>);
BENCHMARK_TEMPLATE(agc_get_max_gain, float, std::complex<float>);
BENCHMARK_TEMPLATE(agc_set_max_gain, float, std::complex<float>);
BENCHMARK_TEMPLATE(agc_get_min_gain, float, std::complex<float>);
BENCHMARK_TEMPLATE(agc_set_min_gain, float, std::complex<float>);
BENCHMARK_TEMPLATE(agc_set_gain_limits, float, std::complex<float>);
BENCHMARK_TEMPLATE(agc_get_scale, float, std::complex<float>);
BENCHMARK_TEMPLATE(agc_set_scale, float, std::complex<float>);
BENCHMARK_TEMPLATE(agc_get_bandwidth, float, std::complex<float>);
BENCHMARK_TEMPLATE(agc_set_bandwidth, float, std::complex<float>);
BENCHMARK_TEMPLATE(agc_get_rssi, float, std::complex<float>);
BENCHMARK_TEMPLATE(agc_set_rssi, float, std::complex<float>);
BENCHMARK_TEMPLATE(agc_get_gain, float, std::complex<float>);
BENCHMARK_TEMPLATE(agc_set_gain, float, std::complex<float>);
BENCHMARK_TEMPLATE(agc_lock, float, std::complex<float>);
BENCHMARK_TEMPLATE(agc_unlock, float, std::complex<float>);
BENCHMARK_TEMPLATE(agc_locked, float, std::complex<float>);
BENCHMARK_TEMPLATE(agc_execute_one, float, std::complex<float>);
BENCHMARK_TEMPLATE(agc_execute_vector_inplace, float, std::complex<float>)->Arg(1024)->Arg(4096);
BENCHMARK_TEMPLATE(agc_execute_vector, float, std::complex<float>)->Arg(1024)->Arg(4096);

// agc_cd
BENCHMARK_TEMPLATE(agc_constructor, double, std::complex<double>);
BENCHMARK_TEMPLATE(agc_get_max_gain, double, std::complex<double>);
BENCHMARK_TEMPLATE(agc_set_max_gain, double, std::complex<double>);
BENCHMARK_TEMPLATE(agc_get_min_gain, double, std::complex<double>);
BENCHMARK_TEMPLATE(agc_set_min_gain, double, std::complex<double>);
BENCHMARK_TEMPLATE(agc_set_gain_limits, double, std::complex<double>);
BENCHMARK_TEMPLATE(agc_get_scale, double, std::complex<double>);
BENCHMARK_TEMPLATE(agc_set_scale, double, std::complex<double>);
BENCHMARK_TEMPLATE(agc_get_bandwidth, double, std::complex<double>);
BENCHMARK_TEMPLATE(agc_set_bandwidth, double, std::complex<double>);
BENCHMARK_TEMPLATE(agc_get_rssi, double, std::complex<double>);
BENCHMARK_TEMPLATE(agc_set_rssi, double, std::complex<double>);
BENCHMARK_TEMPLATE(agc_get_gain, double, std::complex<double>);
BENCHMARK_TEMPLATE(agc_set_gain, double, std::complex<double>);
BENCHMARK_TEMPLATE(agc_lock, double, std::complex<double>);
BENCHMARK_TEMPLATE(agc_unlock, double, std::complex<double>);
BENCHMARK_TEMPLATE(agc_locked, double, std::complex<double>);
BENCHMARK_TEMPLATE(agc_execute_one, double, std::complex<double>);
BENCHMARK_TEMPLATE(agc_execute_vector_inplace, double, std::complex<double>)->Arg(1024)->Arg(4096);
BENCHMARK_TEMPLATE(agc_execute_vector, double, std::complex<double>)->Arg(1024)->Arg(4096);
