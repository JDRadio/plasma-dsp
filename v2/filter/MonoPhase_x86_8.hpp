////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_FILTER_MONOPHASE_X86_8_HPP
#define PLASMA_FILTER_MONOPHASE_X86_8_HPP

#include "optim/SIMD.hpp"

namespace plasma {

template <>
class MonoPhase<optim::Arch_x86, optim::Size_8> : public Filter
{
protected:
    //! \note _mm256_load_ps: AVX
    //! \note _mm256_fmadd_ps: FMA
    //! \note sum8: SSE, SSE3, AVX
    template <typename OutputIt>
    [[nodiscard]] OutputIt extract(OutputIt out) noexcept
    {
        __m256 accReal{0};
        __m256 accImag{0};

        for (unsigned int n = 0; n < (aligned_taps_ & ~7); n += 8) {
            __m256 delayReal = _mm256_load_ps(delay_line_re_ + n);
            __m256 delayImag = _mm256_load_ps(delay_line_im_ + n);
            __m256 coeff = _mm256_load_ps(coefficients_ + n);

            accReal = _mm256_fmadd_ps(delayReal, coeff, accReal);
            accImag = _mm256_fmadd_ps(delayImag, coeff, accImag);
        }

        *(out++) = {SIMD<optim::Arch_x86>::sum8(accReal), SIMD<optim::Arch_x86>::sum8(accImag)};

        return out;
    }
};

} // namespace

#endif
