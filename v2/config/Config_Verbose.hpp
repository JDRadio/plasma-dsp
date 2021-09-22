////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_DSP_CONFIG_CONFIG_VERBOSE_HPP
#define PLASMA_DSP_CONFIG_CONFIG_VERBOSE_HPP

#if __ARM_NEON
    #pragma message "__ARM_NEON=1"
#else
    #pragma message "__ARM_NEON=0"
#endif

#if __SSE__
    #pragma message "__SSE__=1"
#else
    #pragma message "__SSE__=0"
#endif

#if __SSE3__
    #pragma message "__SSE3__=1"
#else
    #pragma message "__SSE3__=0"
#endif

#if __FMA__
    #pragma message "__FMA__=1"
#else
    #pragma message "__FMA__=0"
#endif

#if __AVX__
    #pragma message "__AVX__=1"
#else
    #pragma message "__AVX__=0"
#endif

#if PLASMA_HAVE_NEON
    #pragma message "PLASMA_HAVE_NEON=1"
#else
    #pragma message "PLASMA_HAVE_NEON=0"
#endif

#if PLASMA_HAVE_X86
    #pragma message "PLASMA_HAVE_X86=1"
#else
    #pragma message "PLASMA_HAVE_X86=0"
#endif

#if PLASMA_HAVE_X86_AVX
    #pragma message "PLASMA_HAVE_X86_AVX=1"
#else
    #pragma message "PLASMA_HAVE_X86_AVX=0"
#endif

#endif
