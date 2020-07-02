////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the AmateurRadio.Engineer project
//! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
//! \date 2020
//! \copyright AmateurRadio.Engineer
////////////////////////////////////////////////////////////////////////////////
template <typename T>
NCO<T>::NCO(void)
    : theta_(0)
    , d_theta_(0)
{
}

template <typename T>
double NCO<T>::get_angular_frequency(void) const
{
    return d_theta_;
}

template <typename T>
double NCO<T>::get_frequency(void) const
{
    return d_theta_ / (2.0 * MATH<T>::pi);
}

template <typename T>
void NCO<T>::set_angular_frequency(double frequency)
{
    d_theta_ = frequency;
}

template <typename T>
void NCO<T>::set_frequency(double frequency)
{
    d_theta_ = 2.0 * MATH<T>::pi * frequency;
}

template <typename T>
void NCO<T>::adjust_angular_frequency(double dw)
{
    d_theta_ += dw;
}

template <typename T>
void NCO<T>::adjust_frequency(double df)
{
    d_theta_ += 2.0 * MATH<T>::pi * df;
}

template <typename T>
double NCO<T>::get_phase(void) const
{
    return theta_;
}

template <typename T>
void NCO<T>::set_phase(double phase)
{
    theta_ = phase;
    constrain_phase();
}

template <typename T>
void NCO<T>::adjust_phase(double dp)
{
    theta_ += dp;
}

template <typename T>
void NCO<T>::constrain_phase(void)
{
    while (theta_ >= MATH<T>::two_pi) {
        theta_ -= MATH<T>::two_pi;
    }

    while (theta_ < 0) {
        theta_ += MATH<T>::two_pi;
    }
}

template <typename T>
void NCO<T>::step(void)
{
    theta_ += d_theta_;
    constrain_phase();
}

template <typename T>
void NCO<T>::reset(void)
{
    theta_ = 0;
    d_theta_ = 0;
}

template <typename T>
std::complex<T> NCO<T>::mix_up(std::complex<T> x)
{
    // std::exp() is very slow, let's use the following equivalence
    // e^ix = cos(x) + i sin(x)
    return x * std::complex<T>(std::cos(theta_), std::sin(theta_));
}

template <typename T>
std::complex<T> NCO<T>::mix_down(std::complex<T> x)
{
    // std::exp() is very slow, let's use the following equivalence
    // e^-ix = cos(-x) + i sin(-x) = cos(x) - i sin(x)
    return x * std::complex<T>(std::cos(theta_), -std::sin(theta_));
}

template <typename T>
std::vector<std::complex<T>> NCO<T>::mix_up(std::vector<std::complex<T>> const& in)
{
    std::vector<std::complex<T>> out(in.size());

    for (std::size_t i = 0; i < in.size(); ++i) {
        out[i] = mix_up(in[i]);
        step();
    }

    return out;
}

template <typename T>
std::vector<std::complex<T>> NCO<T>::mix_down(std::vector<std::complex<T>> const& in)
{
    std::vector<std::complex<T>> out(in.size());

    for (std::size_t i = 0; i < in.size(); ++i) {
        out[i] = mix_down(in[i]);
        step();
    }

    return out;
}

template <typename T>
void NCO<T>::set_pll_bandwidth(double bandwidth)
{
    alpha_ = bandwidth;
    beta_ = std::sqrt(bandwidth);
}

template <typename T>
void NCO<T>::pll_step(double dphi)
{
    adjust_frequency(alpha_ * dphi);
    adjust_phase(beta_ * dphi);
}
