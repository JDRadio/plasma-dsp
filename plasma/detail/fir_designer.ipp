////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the AmateurRadio.Engineer project
//! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
//! \date 2020
//! \copyright AmateurRadio.Engineer
////////////////////////////////////////////////////////////////////////////////
template <typename T, typename T_TAPS>
unsigned int FIR_DESIGNER<T,T_TAPS>::kaiser_order(double att, double df)
{
    unsigned int n;

    if (att >= 21.0) {
        n = std::ceil((att - 7.95) / (2.285 * (2.0 * MATH<double>::pi * df)));
    }
    else {
        n = std::ceil(5.79 / (2.0 * MATH<double>::pi * df));
    }

    if (n % 2 == 0) {
        return n + 1;
    }

    return n;
}

template <typename T, typename T_TAPS>
double FIR_DESIGNER<T,T_TAPS>::kaiser_beta(double att)
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

template <typename T, typename T_TAPS>
double FIR_DESIGNER<T,T_TAPS>::kaiser_window(double beta, int n, int big_n)
{
    double t = static_cast<double>(n) - static_cast<double>(big_n - 1) / 2.0;
    double r = 2.0 * t / static_cast<double>(big_n - 1);
    double a = std::cyl_bessel_i(0, beta * std::sqrt(1.0 - r * r));
    double b = std::cyl_bessel_i(0, beta);

    return a / b;
}

template <typename T, typename T_TAPS>
std::vector<T_TAPS> FIR_DESIGNER<T,T_TAPS>::create_kaiser_taps(double fc, double df, double att)
{
    double beta = kaiser_beta(att);
    unsigned int len = kaiser_order(att, df);
    std::vector<T_TAPS> taps(len);

    for (std::size_t n = 0; n < len; n++) {
        double t = n - (len - 1) / 2.0;
        taps[n] = 2.0 * fc * MATH<double>::sinc(2.0 * fc * t);
        taps[n] *= kaiser_window(beta, n, len);
    }

    return taps;
}

template <typename T, typename T_TAPS>
FIR<T,T_TAPS> FIR_DESIGNER<T,T_TAPS>::create_kaiser_filter(double fc, double df, double att)
{
    FIR<T,T_TAPS> filter;
    filter.set_taps(create_kaiser_taps(fc, df, att));
    return filter;
}

template <typename T, typename T_TAPS>
FIR_INTERPOLATOR<T,T_TAPS> FIR_DESIGNER<T,T_TAPS>::create_kaiser_interpolator(unsigned int k, double df, double att)
{
    FIR_INTERPOLATOR<T,T_TAPS> filter;
    filter.set_factor(k);
    filter.set_taps(create_kaiser_taps(0.5 / k, df, att));
    return filter;
}

template <typename T, typename T_TAPS>
FIR_DECIMATOR<T,T_TAPS> FIR_DESIGNER<T,T_TAPS>::create_kaiser_decimator(unsigned int k, double df, double att)
{
    FIR_DECIMATOR<T,T_TAPS> filter;
    filter.set_factor(k);
    filter.set_taps(create_kaiser_taps(0.5 / k, df, att));
    return filter;
}

template <typename T, typename T_TAPS>
std::vector<T_TAPS> FIR_DESIGNER<T,T_TAPS>::create_rrc_taps(unsigned int k, unsigned int m, double r)
{
    unsigned int len = 2 * k * m + 1;
    std::vector<T_TAPS> h(len);

    double s = std::sin(MATH<double>::pi / (4 * r));
    double c = std::cos(MATH<double>::pi / (4 * r));

    for (std::size_t n = 0; n < len; n++) {
        double t = static_cast<double>(n) / static_cast<double>(k) - m;

        if (std::abs(t) < 1e-6) {
            // t = 0
            h[n] = (1 - r) + 4 * r / MATH<double>::pi;
        }
        else {
            double p = 1 - (16 * r*r * t*t);

            if (std::abs(p) < 1e-6) {
                // t = +/- Tc/(4r)
                h[n] = r / MATH<double>::sqrt2 * ((1 + 2 / MATH<double>::pi) * s + (1 - 2 / MATH<double>::pi) * c);
            }
            else {
                // t = otherwise
                h[n] = (std::sin(MATH<double>::pi * t * (1 - r)) + 4 * r * t * std::cos(MATH<double>::pi * t * (1 + r))) / (MATH<double>::pi * t * p);
            }
        }
    }

    return h;
}

template <typename T, typename T_TAPS>
FIR_INTERPOLATOR<T,T_TAPS> FIR_DESIGNER<T,T_TAPS>::create_rrc_interpolator(unsigned int k, unsigned int m, double r)
{
    FIR_INTERPOLATOR<T,T_TAPS> filter;
    filter.set_factor(k);
    filter.set_taps(create_rrc_taps(k, m, r));
    return filter;
}

template <typename T, typename T_TAPS>
FIR_DECIMATOR<T,T_TAPS> FIR_DESIGNER<T,T_TAPS>::create_rrc_decimator(unsigned int k, unsigned int m, double r)
{
    FIR_DECIMATOR<T,T_TAPS> filter;
    filter.set_factor(k);
    filter.set_taps(create_rrc_taps(k, m, r));
    return filter;
}
