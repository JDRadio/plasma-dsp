////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the AmateurRadio.Engineer project
//! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
//! \date 2020
//! \copyright AmateurRadio.Engineer
////////////////////////////////////////////////////////////////////////////////
template <typename T>
NCO_TABLE<T>::NCO_TABLE(void)
    : theta_(0)
    , d_theta_(0)
    , alpha_(0.1)
    , beta_(std::sqrt(0.1))
    , table_()
{
    T val = MATH<T>::two_pi / static_cast<T>(table_.size());

    for (std::size_t i = 0; i < table_.size(); ++i) {
        table_[i] = std::complex<T>(
            std::cos(i * val),
            std::sin(i * val)
        );
    }
}

template <typename T>
double NCO_TABLE<T>::get_angular_frequency(void) const
{
    double freq = MATH<T>::two_pi * d_theta_ / static_cast<double>(1UL<<32);
    return freq > MATH<T>::pi ? freq - MATH<T>::two_pi : freq;
}

template <typename T>
double NCO_TABLE<T>::get_frequency(void) const
{
    double freq = d_theta_ / static_cast<double>(1UL<<32);
    return freq > 0.5 ? freq - 1.0 : freq;
}

template <typename T>
void NCO_TABLE<T>::set_angular_frequency(double w)
{
    d_theta_ = constrain_value(w);
}

template <typename T>
void NCO_TABLE<T>::set_frequency(double frequency)
{
    d_theta_ = constrain_value(MATH<T>::two_pi * frequency);
}

template <typename T>
void NCO_TABLE<T>::adjust_angular_frequency(double dw)
{
    d_theta_ += constrain_value(dw);
}

template <typename T>
void NCO_TABLE<T>::adjust_frequency(double df)
{
    d_theta_ += constrain_value(MATH<T>::two_pi * df);
}

template <typename T>
double NCO_TABLE<T>::get_phase(void) const
{
    return MATH<T>::two_pi * d_theta_ / static_cast<double>(1UL<<32);
}

template <typename T>
void NCO_TABLE<T>::set_phase(double phase)
{
    theta_ = constrain_value(phase);
}

template <typename T>
void NCO_TABLE<T>::adjust_phase(double dp)
{
    theta_ += constrain_value(dp);
}

template <typename T>
std::uint32_t NCO_TABLE<T>::constrain_value(double val) const
{
    val -= static_cast<long>(val / MATH<T>::two_pi);

    if (val < 0) {
        val += 1;
    }

    return static_cast<std::uint32_t>(val * UINT32_MAX);
}

template <typename T>
void NCO_TABLE<T>::step(void)
{
    theta_ += d_theta_;
}

template <typename T>
std::complex<T> NCO_TABLE<T>::get_value(void) const
{
    return table_[
        (
            (theta_ + (1 << (31 - SAMPLE_COUNT_BITS)))
            >> (32 - SAMPLE_COUNT_BITS)
        )
        & ((1 << SAMPLE_COUNT_BITS) - 1)
    ];
}

template <typename T>
void NCO_TABLE<T>::reset(void)
{
    theta_ = 0;
    d_theta_ = 0;
}

template <typename T>
std::complex<T> NCO_TABLE<T>::mix_up(std::complex<T> x)
{
    return x * get_value();
}

template <typename T>
std::complex<T> NCO_TABLE<T>::mix_down(std::complex<T> x)
{
    return x * std::conj(get_value());
}

template <typename T>
std::vector<std::complex<T>> NCO_TABLE<T>::mix_up(std::vector<std::complex<T>> const& in)
{
    std::vector<std::complex<T>> out(in.size());

    for (std::size_t i = 0; i < in.size(); ++i) {
        out[i] = mix_up(in[i]);
        step();
    }

    return out;
}

template <typename T>
std::vector<std::complex<T>> NCO_TABLE<T>::mix_down(std::vector<std::complex<T>> const& in)
{
    std::vector<std::complex<T>> out(in.size());

    for (std::size_t i = 0; i < in.size(); ++i) {
        out[i] = mix_down(in[i]);
        step();
    }

    return out;
}

template <typename T>
void NCO_TABLE<T>::set_pll_bandwidth(double bandwidth)
{
    alpha_ = bandwidth;
    beta_ = std::sqrt(bandwidth);
}

template <typename T>
void NCO_TABLE<T>::pll_step(double dphi)
{
    adjust_frequency(alpha_ * dphi);
    adjust_phase(beta_ * dphi);
}
