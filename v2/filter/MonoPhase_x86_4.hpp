////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_FILTER_MONOPHASE_X86_4_HPP
#define PLASMA_FILTER_MONOPHASE_X86_4_HPP

#include "optim/SIMD.hpp"

namespace plasma {

template <>
class MonoPhase<optim::Arch_x86, optim::Size_4> : public Filter
{
protected:
    //! \note _mm_load_ps: SSE
    //! \note _mm_fmadd_ps: FMA
    //! \note simd::sum4: SSE, SSE3
    template <typename OutputIt>
    [[nodiscard]] OutputIt extract(OutputIt out) noexcept
    {
        __m128 accReal{0};
        __m128 accImag{0};

        for (unsigned int n = 0; n < (aligned_taps_ & ~3); n += 4) {
            __m128 delayReal = _mm_load_ps(delay_line_re_ + n);
            __m128 delayImag = _mm_load_ps(delay_line_im_ + n);
            __m128 coeff = _mm_load_ps(coefficients_ + n);

            accReal = _mm_fmadd_ps(delayReal, coeff, accReal);
            accImag = _mm_fmadd_ps(delayImag, coeff, accImag);
        }

        *(out++) = {SIMD<optim::Arch_x86>::sum4(accReal), SIMD<optim::Arch_x86>::sum4(accImag)};

        return out;
    }
};

} // namespace

#endif
