////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_DSP_CONFIG_CONFIG_HPP
#define PLASMA_DSP_CONFIG_CONFIG_HPP

// ARM optimizations
#if __ARM_NEON
    #define PLASMA_HAVE_NEON 1
#else
    #define PLASMA_HAVE_NEON 0
#endif

// x86 optimizations
#if __SSE__ && __SSE3__ && __FMA__
    #define PLASMA_HAVE_X86 1

    #if __AVX__
        #define PLASMA_HAVE_X86_AVX 1
    #else
        #define PLASMA_HAVE_X86_AVX 0
    #endif
#else
    #define PLASMA_HAVE_X86 0
    #define PLASMA_HAVE_X86_AVX 0
#endif

// Verbose configuration
#ifdef PLASMA_CONFIG_VERBOSE
    #include "Config_Verbose.hpp"
#endif

#endif
