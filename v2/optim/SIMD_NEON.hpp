////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_DSP_OPTIM_SIMD_NEON_HPP
#define PLASMA_DSP_OPTIM_SIMD_NEON_HPP

#include <arm/arm_neon.h>

namespace plasma {

//! \todo 2021-09-21: Implement the NEON optimizations
template <>
class SIMD<optim::Arch_NEON>
{
public:
    //! \note _mm_cvtss_f32: SSE
    //! \note _mm_hadd_ps: SSE3
    [[nodiscard]]
    static inline float sum4(float32x4_t x) noexcept
    {
        // x = [1, 2, 3, 4]
        x = _mm_hadd_ps(x, x);
        // x = [1+2, 3+4, 1+2, 3+4]
        x = _mm_hadd_ps(x, x);
        // x = [1+2+3+4, 1+2+3+4, 1+2+3+4, 1+2+3+4]

        return _mm_cvtss_f32(x);
        // return [1+2+3+4]
    }

    //! \note _mm_add_ps: SSE
    //! \note _mm256_castps256_ps128: AVX
    //! \note _mm256_extractf128_ps: AVX
    //! \note sum4: SSE, SSE3
    [[nodiscard]]
    static inline float sum8(float32x4_t x) noexcept
    {
        return sum4(_mm_add_ps(_mm256_castps256_ps128(x), _mm256_extractf128_ps(x, 1)));
    }
};

} // namespace

#endif
