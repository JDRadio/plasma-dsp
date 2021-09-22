////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_DSP_OPTIM_OPTIM_HPP
#define PLASMA_DSP_OPTIM_OPTIM_HPP

namespace plasma::optim {

struct Size_None {};
struct Size_4 {};
struct Size_8 {};

struct Arch_x86 {};
struct Arch_NEON {};
struct Arch_None {};

} // namespace

#endif
