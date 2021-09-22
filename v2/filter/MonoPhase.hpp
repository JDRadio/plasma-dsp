////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_FILTER_MONOPHASE_HPP
#define PLASMA_FILTER_MONOPHASE_HPP

#include "optim/Optim.hpp"
#include <type_traits>

namespace plasma {

class Filter
{
protected:
    unsigned int aligned_taps_;
    float* delay_line_re_;
    float* delay_line_im_;
    float* coefficients_;
};

template <typename OptimArch, typename OptimSize>
class MonoPhase : public Filter
{
    static_assert(
        std::disjunction_v<
            std::conjunction<
                std::is_same<OptimArch, optim::Arch_None>,
                std::is_same<OptimSize, optim::Size_None>
            >,
            std::conjunction<
                std::is_same<OptimArch, optim::Arch_x86>,
                std::disjunction<
                    std::is_same<OptimSize, optim::Size_4>,
                    std::is_same<OptimSize, optim::Size_8>
                >
            >,
            std::conjunction<
                std::is_same<OptimArch, optim::Arch_NEON>,
                std::disjunction<
                    std::is_same<OptimSize, optim::Size_4>,
                    std::is_same<OptimSize, optim::Size_8>
                >
            >
        >,
        "Invalid combination of optimization settings"
    );
};

} // namespace

#include "MonoPhase_None.hpp"
#include "MonoPhase_x86_4.hpp"
#include "MonoPhase_x86_8.hpp"

#endif
