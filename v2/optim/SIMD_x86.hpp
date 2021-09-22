////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_DSP_OPTIM_SIMD_X86_HPP
#define PLASMA_DSP_OPTIM_SIMD_X86_HPP

#include <pmmintrin.h>
#include <immintrin.h>

namespace plasma {

template <>
class SIMD<optim::Arch_x86>
{
public:
    //! \note _mm_cvtss_f32: SSE
    //! \note _mm_hadd_ps: SSE3
    [[nodiscard]]
    static inline float sum4(__m128 x) noexcept
    {
        x = _mm_hadd_ps(x, x);
        x = _mm_hadd_ps(x, x);

        return _mm_cvtss_f32(x);
    }

    //! \note _mm_add_ps: SSE
    //! \note _mm256_castps256_ps128: AVX
    //! \note _mm256_extractf128_ps: AVX
    //! \note sum4: SSE, SSE3
    [[nodiscard]]
    static inline float sum8(__m256 x) noexcept
    {
        return sum4(_mm_add_ps(_mm256_castps256_ps128(x), _mm256_extractf128_ps(x, 1)));
    }
};

} // namespace

#endif
