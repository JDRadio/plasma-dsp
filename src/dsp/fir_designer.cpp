#include "plasma/dsp/fir_designer.hpp"
#include "plasma/dsp/math.hpp"
#include <cmath>

namespace dsp {

unsigned int fir_designer::kaiser_order(double att, double df)
{
    unsigned int n;

    if (att >= 21.0) {
        n = ceil((att - 7.95) / (2.285 * (2.0 * math::pi * df)));
    }
    else {
        n = ceil(5.79 / (2.0 * math::pi * df));
    }

    if (n % 2 == 0) {
        return n + 1;
    }

    return n;
}

double fir_designer::kaiser_beta(double att)
{
    att = abs(att);

    if (att > 50.0) {
        return 0.1102 * (att - 8.7);
    }
    else if (att < 21.0) {
        return 0.0;
    }
    else {
        return 0.5842 * pow(att - 21.0, 0.4) + 0.07886 * (att - 21.0);
    }
}

double fir_designer::kaiser_window(double beta, int n, int big_n)
{
    double t = static_cast<double>(n) - static_cast<double>(big_n - 1) / 2.0;
    double r = 2.0 * t / static_cast<double>(big_n - 1);
    double a = cyl_bessel_i(0, beta * sqrt(1.0 - r * r));
    double b = cyl_bessel_i(0, beta);

    return a / b;
}

vector<double> fir_designer::create_kaiser(double fc, double df, double att)
{
    double beta = kaiser_beta(att);
    unsigned int len = kaiser_order(att, df);
    vector<double> taps(len);

    for (size_t n = 0; n < len; n++) {
        double t = n - (len - 1) / 2.0;
        taps[n] = 2.0 * fc * math::sinc(2.0 * fc * t);
        taps[n] *= kaiser_window(beta, n, len);
    }

    return taps;
}

vector<double> fir_designer::create_rrc(unsigned int k, unsigned int m, double r)
{
    unsigned int len = 2 * k * m + 1;
    vector<double> h(len);

    double s = sin(math::pi / (4 * r));
    double c = cos(math::pi / (4 * r));

    for (size_t n = 0; n < len; n++) {
        double t = static_cast<double>(n) / static_cast<double>(k) - m;

        if (abs(t) < 1e-6) {
            // t = 0
            h[n] = (1 - r) + 4 * r / math::pi;
        }
        else {
            double p = 1 - (16 * r*r * t*t);

            if (abs(p) < 1e-6) {
                // t = +/- Tc/(4r)
                h[n] = r / math::sqrt2 * ((1 + 2 / math::pi) * s + (1 - 2 / math::pi) * c);
            }
            else {
                // t = otherwise
                h[n] = (sin(math::pi * t * (1 - r)) + 4 * r * t * cos(math::pi * t * (1 + r))) / (math::pi * t * p);
            }
        }
    }

    return h;
}

} // namespace
