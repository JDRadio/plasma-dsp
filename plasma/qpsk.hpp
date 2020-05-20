////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the AmateurRadio.Engineer project
//! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
//! \date 2020
//! \copyright AmateurRadio.Engineer
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_QPSK_HPP
#define PLASMA_QPSK_HPP

#include <complex>
#include <vector>
#include <string>
#include <cstdint>

namespace plasma {

template <typename T>
class QPSK {
public:
    QPSK(void);
    ~QPSK(void) = default;

    //! Modulates symbols
    //! \param[in] in Symbols to modulate (4 symbols per byte)
    //! \return Output buffer of modulated samples
    std::vector<std::complex<T>> modulate(std::string const& in);

    //! Demodulates samples
    //! \param[in] in Samples to demodulate
    //! \return Demodulated symboles (4 symbols per byte)
    std::string demodulate(std::vector<std::complex<T>> const& in);

    //! Modulates a single symbol
    //! \param[in] sym Symbol to modulate
    //! \return Modulated sample
    std::complex<T> modulate(std::uint8_t sym);

    //! Modulates symbols
    //! \param[in] in Symbols to modulate (1 symbol per byte)
    //! \param[in] in_size Number of bytes
    //! \param[out] out Output buffer of modulated samples
    void modulate(const std::uint8_t* in, std::size_t in_size, std::complex<T>* out);

    //! Demodulates a single sample
    //! \param[in] sig Sample to demodulate
    //! \return Demodulated symbol
    std::uint8_t demodulate(std::complex<T> sig);

    //! Demodulates samples
    //! \param[in] in Samples to demodulate
    //! \param[in] in_size Number of samples
    //! \param[out] out Output buffer of demodulated symbols (1 symbol per byte)
    void demodulate(const std::complex<T>* in, std::size_t in_size, std::uint8_t* out);

    //! Retrieves the re-modulated sample for the last demodulated symbol
    //! \return Constellation-aligned sample
    std::complex<T> get_demodulator_sample(void) const;

    //! Retrieves the phase error of the last demodulated symbol
    //! \return Phase error
    double get_demodulator_phase_error(void) const;

    //! Retrieves the error vector magnitude (EVM) of the last demodulated symbol
    //! \return EVM
    double get_demodulator_evm(void) const;

    //! Retrieves the error vector magnitude (EVM) in decibels (dB) of the last demodulated symbol
    //! \return EVM(dB)
    double get_demodulator_evm_db(void) const;

private:
    std::complex<T> r_;
    std::complex<T> x_hat_;
};

#include "detail/qpsk.ipp"

using qpsk_f = QPSK<float>;
using qpsk_d = QPSK<double>;

} // namespace

#endif
