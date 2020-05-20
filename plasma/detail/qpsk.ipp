////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the AmateurRadio.Engineer project
//! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
//! \date 2020
//! \copyright AmateurRadio.Engineer
////////////////////////////////////////////////////////////////////////////////
template <typename T>
QPSK<T>::QPSK(void)
    : r_(0)
    , x_hat_(0)
{
}

template <typename T>
std::vector<std::complex<T>> QPSK<T>::modulate(std::string const& in)
{
    std::vector<std::complex<T>> out(in.size() * 4);

    for (std::size_t i = 0; i < in.size(); i++) {
        out[i*4+0] = modulate(in[i] >> 6);
        out[i*4+1] = modulate(in[i] >> 4);
        out[i*4+2] = modulate(in[i] >> 2);
        out[i*4+3] = modulate(in[i]);
    }

    return out;
}

template <typename T>
std::string QPSK<T>::demodulate(std::vector<std::complex<T>> const& in)
{
    std::size_t count = in.size() / 4 + (in.size() % 4 > 0 ? 1 : 0);
    std::string out;
    out.reserve(count);

    for (std::size_t i = 0; i < count; i++) {
        int a = demodulate(in[i*4+0]);
        int b = 0;
        int c = 0;
        int d = 0;

        if (i + 1 < in.size()) {
            b = demodulate(in[i*4+1]);
        }

        if (i + 2 < in.size()) {
            c = demodulate(in[i*4+2]);
        }

        if (i + 3 < in.size()) {
            d = demodulate(in[i*4+3]);
        }

        out.push_back((a << 6) | (b << 4) | (c << 2) | d);
    }

    return out;
}

template <typename T>
std::complex<T> QPSK<T>::modulate(std::uint8_t sym)
{
    return std::complex<T>(
        (sym & 1) == 0 ? -M_SQRT1_2 : M_SQRT1_2,
        (sym & 2) == 0 ? -M_SQRT1_2 : M_SQRT1_2
    );
}

template <typename T>
void QPSK<T>::modulate(const std::uint8_t* in, std::size_t in_size, std::complex<T>* out)
{
    for (std::size_t i = 0; i < in_size; i++) {
        out[i] = modulate(in[i]);
    }
}

template <typename T>
std::uint8_t QPSK<T>::demodulate(std::complex<T> sig)
{
    std::uint8_t sym = (sig.real() < 0 ? 0 : 1) | (sig.imag() < 0 ? 0 : 2);

    r_ = sig;
    x_hat_ = modulate(sym);

    return sym;
}

template <typename T>
void QPSK<T>::demodulate(const std::complex<T>* in, std::size_t in_size, std::uint8_t* out)
{
    for (std::size_t i = 0; i < in_size; i++) {
        out[i] = demodulate(in[i]);
    }
}

template <typename T>
std::complex<T> QPSK<T>::get_demodulator_sample(void) const
{
    return x_hat_;
}

template <typename T>
double QPSK<T>::get_demodulator_phase_error(void) const
{
    return std::arg(r_ * std::conj(x_hat_));
}

template <typename T>
double QPSK<T>::get_demodulator_evm(void) const
{
    return std::abs(x_hat_ - r_);
}

template <typename T>
double QPSK<T>::get_demodulator_evm_db(void) const
{
    return -20. * std::log10(get_demodulator_evm());
}
