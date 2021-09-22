////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_DSP_OPTIM_SIMD_HPP
#define PLASMA_DSP_OPTIM_SIMD_HPP

#include "config/Config.hpp"

namespace plasma {

template <typename OptimArch>
class SIMD
{
    static_assert(
        std::disjunction_v<
            std::is_same<OptimArch, optim::Arch_x86>,
            std::is_same<OptimArch, optim::Arch_NEON>
        >,
        "Only the following SIMD instruction sets are supported: x86, NEON"
    );
};

} // namespace

#if PLASMA_HAVE_NEON
#include "SIMD_NEON.hpp"
#endif

#if PLASMA_HAVE_X86
#include "SIMD_x86.hpp"
#endif

#endif
