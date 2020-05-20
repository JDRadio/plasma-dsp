////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the AmateurRadio.Engineer project
//! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
//! \date 2020
//! \copyright AmateurRadio.Engineer
////////////////////////////////////////////////////////////////////////////////
template <typename T, typename T_TAPS>
void FIR<T,T_TAPS>::set_taps(std::vector<T_TAPS> const& taps)
{
    taps_.clear();
    taps_.reserve(taps.size());
    taps_.insert(taps_.end(), taps.rbegin(), taps.rend());

    samples_.clear();
    samples_.resize(taps_.size(), 0);
}

template <typename T, typename T_TAPS>
unsigned int FIR<T,T_TAPS>::get_delay(void) const
{
    return taps_.size() / 2;
}

template <typename T, typename T_TAPS>
void FIR<T,T_TAPS>::reset(void)
{
    samples_.clear();
    samples_.resize(taps_.size(), 0);
}

template <typename T, typename T_TAPS>
void FIR<T,T_TAPS>::push(T x)
{
    samples_.erase(samples_.begin());
    samples_.push_back(x);
}

template <typename T, typename T_TAPS>
T FIR<T,T_TAPS>::execute(void)
{
    T v = 0;

    for (std::size_t i = 0; i < taps_.size(); i++) {
        v += taps_[i] * samples_[i];
    }

    return v;
}

template <typename T, typename T_TAPS>
std::vector<T> FIR<T,T_TAPS>::execute(std::vector<T> const& in)
{
    std::vector<T> out(in.size());

    for (std::size_t i = 0; i < in.size(); i++) {
        push(in[i]);
        out[i] = execute();
    }

    return out;
}
