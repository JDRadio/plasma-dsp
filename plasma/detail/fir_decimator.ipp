////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the AmateurRadio.Engineer project
//! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
//! \date 2020
//! \copyright AmateurRadio.Engineer
////////////////////////////////////////////////////////////////////////////////
template <typename T, typename T_TAPS>
FIR_DECIMATOR<T,T_TAPS>::FIR_DECIMATOR(void)
    : factor_(1)
    , samples_(0)
    , filter_()
{
}

template <typename T, typename T_TAPS>
void FIR_DECIMATOR<T,T_TAPS>::set_taps(std::vector<T_TAPS> const& taps)
{
    filter_.set_taps(taps);
}

template <typename T, typename T_TAPS>
void FIR_DECIMATOR<T,T_TAPS>::set_factor(unsigned int factor)
{
    factor_ = factor;
    samples_ = 0;
}

template <typename T, typename T_TAPS>
unsigned int FIR_DECIMATOR<T,T_TAPS>::get_factor(void) const
{
    return factor_;
}

template <typename T, typename T_TAPS>
void FIR_DECIMATOR<T,T_TAPS>::reset(void)
{
    samples_ = 0;
    filter_.reset();
}

template <typename T, typename T_TAPS>
std::vector<T> FIR_DECIMATOR<T,T_TAPS>::execute(std::vector<T> const& in)
{
    if (factor_ == 1) {
        return in;
    }

    std::vector<T> out;
    out.reserve(in.size() / factor_ + 2);

    for (std::size_t i = 0; i < in.size(); i++) {
        filter_.push(in[i]);

        if (samples_++ % factor_ == 0) {
            out.push_back(filter_.execute());
            samples_ %= factor_;
        }
    }

    return out;
}
