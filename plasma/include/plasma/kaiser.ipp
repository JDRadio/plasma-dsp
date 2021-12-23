////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
namespace plasma {

template <typename T>
unsigned int kaiser<T>::get_order(double att, double df) noexcept
{
    unsigned int n;

    if (att >= 21.0) {
        n = std::ceil((att - 7.95) / (2.285 * (2.0 * math::PI * df)));
    }
    else {
        n = std::ceil(5.79 / (2.0 * math::PI * df));
    }

    if (n % 2 == 0) {
        return n + 1;
    }

    return n;
}

template <typename T>
double kaiser<T>::calculate_beta(double att) noexcept
{
    att = std::abs(att);

    if (att > 50.0) {
        return 0.1102 * (att - 8.7);
    }
    else if (att < 21.0) {
        return 0.0;
    }
    else {
        return 0.5842 * std::pow(att - 21.0, 0.4) + 0.07886 * (att - 21.0);
    }
}

template <typename T>
double kaiser<T>::window(double beta, int n, int big_n) noexcept
{
    double t = static_cast<double>(n) - static_cast<double>(big_n - 1) / 2.0;
    double r = 2.0 * t / static_cast<double>(big_n - 1);
    double a = std::cyl_bessel_i(0, beta * std::sqrt(1.0 - r * r));
    double b = std::cyl_bessel_i(0, beta);

    return a / b;
}

template <typename T>
std::vector<T> kaiser<T>::create_taps(double fc, double df, double att) noexcept
{
    double beta = calculate_beta(att);
    unsigned int len = get_order(att, df);
    std::vector<T> taps(len);

    for (unsigned int n = 0; n < len; ++n) {
        double t = n - (len - 1) / 2.0;
        taps[n] = 2.0 * fc * math::sinc(2.0 * fc * t) * window(beta, n, len);
    }

    return taps;
}

} // namespace
