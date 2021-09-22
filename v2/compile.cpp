////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#include "filter/MonoPhase.hpp"

namespace plasma {

template class MonoPhase<optim::Arch_None, optim::Size_None>;
template class MonoPhase<optim::Arch_x86, optim::Size_4>;
template class MonoPhase<optim::Arch_x86, optim::Size_8>;
template class MonoPhase<optim::Arch_NEON, optim::Size_4>;
template class MonoPhase<optim::Arch_NEON, optim::Size_8>;

} // namespace
