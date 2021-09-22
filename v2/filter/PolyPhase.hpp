////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_FILTER_POLYPHASE_HPP
#define PLASMA_FILTER_POLYPHASE_HPP

#include "optim/Optim.hpp"
#include <type_traits>

namespace plasma {

template <typename Optim = optim::Detect>
class PolyPhase
{
    static_assert(std::disjunction_v<std::is_same_v<Optim, OptimDetect>, std::is_same_v<Optim, Optim4>, std::is_same_v<Optim, Optim8>>, "Optimization types are: OptimDetect, Optim4, Optim8");

protected:
    unsigned int numPhases_;

    //! \note _mm_load_ps: SSE
    //! \note _mm_fmadd_ps: FMA
    //! \note simd::sum4: SSE, SSE3
    template <typename OutputIt>
    [[nodiscard]] OutputIt extract4_x86(OutputIt out) noexcept
    {
        for (unsigned int k = 0; k < numPhases_; ++k) {
            auto const* coeffs = coefficients_ + k * aligned_taps_;
            unsigned int n = 0;

            __m128 accReal{0};
            __m128 accImag{0};

            while (n < aligned_taps_) {
                __m128 delayReal = _mm_load_ps(delay_line_re_ + n);
                __m128 delayImag = _mm_load_ps(delay_line_im_ + n);
                __m128 coeff = _mm_load_ps(coeffs + n);

                accReal = _mm_fmadd_ps(delayReal, coeff, accReal);
                accImag = _mm_fmadd_ps(delayImag, coeff, accImag);

                n += 4;
            }

            *(out++) = {simd::sum4(accReal), simd::sum4(accImag)};
        }

        return out;
    }
    //! \note _mm_load_ps: SSE
    //! \note _mm_fmadd_ps: FMA
    //! \note simd::sum4: SSE, SSE3
    template <typename OutputIt>
    [[nodiscard]] OutputIt extract4_NEON(OutputIt out) noexcept
    {
        unsigned int n = 0;

        __m128 accReal{0};
        __m128 accImag{0};

        while (n < aligned_taps_) {
            __m128 delayReal = _mm_load_ps(delay_line_re_ + n);
            __m128 delayImag = _mm_load_ps(delay_line_im_ + n);
            __m128 coeff = _mm_load_ps(coefficients_ + n);

            accReal = _mm_fmadd_ps(delayReal, coeff, accReal);
            accImag = _mm_fmadd_ps(delayImag, coeff, accImag);

            n += 4;
        }

        *(out++) = {simd::sum4(accReal), simd::sum4(accImag)};

        return out;
    }

    //! \note _mm256_load_ps: AVX
    //! \note _mm256_fmadd_ps: FMA
    //! \note sum8: SSE, SSE3, AVX
    template <typename OutputIt>
    [[nodiscard]] OutputIt extract8_x86(OutputIt out) noexcept
    {
        unsigned int n = 0;

        __m256 accReal{0};
        __m256 accImag{0};

        while (n < aligned_taps_) {
            __m256 delayReal = _mm256_load_ps(delay_line_re_ + n);
            __m256 delayImag = _mm256_load_ps(delay_line_im_ + n);
            __m256 coeff = _mm256_load_ps(coefficients_ + n);

            accReal = _mm256_fmadd_ps(delayReal, coeff, accReal);
            accImag = _mm256_fmadd_ps(delayImag, coeff, accImag);

            n += 8;
        }

        *(out++) = {simd::sum8(accReal), simd::sum8(accImag)};

        return out;
    }

    //! \note _mm256_load_ps: AVX
    //! \note _mm256_fmadd_ps: FMA
    //! \note sum8: SSE, SSE3, AVX
    template <typename OutputIt>
    [[nodiscard]] OutputIt extract8_NEON(OutputIt out) noexcept
    {
        unsigned int n = 0;

        __m256 accReal{0};
        __m256 accImag{0};

        while (n < aligned_taps_) {
            __m256 delayReal = _mm256_load_ps(delay_line_re_ + n);
            __m256 delayImag = _mm256_load_ps(delay_line_im_ + n);
            __m256 coeff = _mm256_load_ps(coefficients_ + n);

            accReal = _mm256_fmadd_ps(delayReal, coeff, accReal);
            accImag = _mm256_fmadd_ps(delayImag, coeff, accImag);

            n += 8;
        }

        *(out++) = {simd::sum8(accReal), simd::sum8(accImag)};

        return out;
    }
};

} // namespace

#endif
