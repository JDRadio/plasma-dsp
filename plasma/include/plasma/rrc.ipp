////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
namespace plasma {

template <typename T>
std::vector<T> rrc<T>::create_taps(unsigned int k, unsigned int m, double r) noexcept
{
    unsigned int len = 2 * k * m + 1;
    std::vector<T> h(len);

    double s = std::sin(math::PI / (4 * r));
    double c = std::cos(math::PI / (4 * r));

    for (unsigned int n = 0; n < len; n++) {
        double t = static_cast<double>(n) / static_cast<double>(k) - m;

        if (std::abs(t) < 1e-6) {
            // t = 0
            h[n] = (1 - r) + 4 * r / math::PI;
        }
        else {
            double p = 1 - (16 * r*r * t*t);

            if (std::abs(p) < 1e-6) {
                // t = +/- Tc/(4r)
                h[n] = r / math::SQRT2 * ((1 + 2 / math::PI) * s + (1 - 2 / math::PI) * c);
            }
            else {
                // t = otherwise
                h[n] = (std::sin(math::PI * t * (1 - r)) + 4 * r * t * std::cos(math::PI * t * (1 + r))) / (math::PI * t * p);
            }
        }
    }

    return h;
}

} // namespace
