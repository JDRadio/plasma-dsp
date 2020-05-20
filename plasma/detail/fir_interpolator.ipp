////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the AmateurRadio.Engineer project
//! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
//! \date 2020
//! \copyright AmateurRadio.Engineer
////////////////////////////////////////////////////////////////////////////////
template <typename T, typename T_TAPS>
FIR_INTERPOLATOR<T,T_TAPS>::FIR_INTERPOLATOR(void)
    : factor_(1)
    , scale_(1)
    , filter_()
{
}

template <typename T, typename T_TAPS>
void FIR_INTERPOLATOR<T,T_TAPS>::set_taps(std::vector<T_TAPS> const& taps)
{
    filter_.set_taps(taps);
}

template <typename T, typename T_TAPS>
void FIR_INTERPOLATOR<T,T_TAPS>::set_factor(unsigned int factor)
{
    scale_ = factor_ = factor;
}

template <typename T, typename T_TAPS>
unsigned int FIR_INTERPOLATOR<T,T_TAPS>::get_factor(void) const
{
    return factor_;
}

template <typename T, typename T_TAPS>
unsigned int FIR_INTERPOLATOR<T,T_TAPS>::get_delay(void) const
{
    return filter_.get_delay();
}

template <typename T, typename T_TAPS>
void FIR_INTERPOLATOR<T,T_TAPS>::reset(void)
{
    filter_.reset();
}

template <typename T, typename T_TAPS>
std::vector<T> FIR_INTERPOLATOR<T,T_TAPS>::execute(std::vector<T> const& in)
{
    if (factor_ == 1) {
        return in;
    }

    std::vector<T> out;
    out.reserve(in.size() * factor_);

    for (std::size_t i = 0; i < in.size(); i++) {
        filter_.push(in[i]);
        out.push_back(scale_ * filter_.execute());

        for (unsigned int k = 1; k < factor_; k++) {
            filter_.push(0);
            out.push_back(scale_ * filter_.execute());
        }
    }

    return out;
}
