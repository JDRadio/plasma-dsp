////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_FILTER_MONOPHASE_NONE_HPP
#define PLASMA_FILTER_MONOPHASE_NONE_HPP

namespace plasma {

template <>
class MonoPhase<optim::Arch_None, optim::Size_None> : public Filter
{
protected:
    template <typename OutputIt>
    [[nodiscard]] OutputIt extract(OutputIt out) noexcept
    {
        float accReal = 0;
        float accImag = 0;

        for (unsigned int n = 0; n < aligned_taps_; ++n) {
            float delayReal = delay_line_re_[n];
            float delayImag = delay_line_im_[n];
            float coeff = coefficients_[n];

            accReal += delayReal * coeff;
            accImag += delayImag * coeff;
        }

        *(out++) = {accReal, accImag};

        return out;
    }
};

} // namespace

#endif
