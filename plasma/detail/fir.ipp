////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the AmateurRadio.Engineer project
//! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
//! \date 2020
//! \copyright AmateurRadio.Engineer
////////////////////////////////////////////////////////////////////////////////
template <typename T, typename T_TAPS>
FIR<T,T_TAPS>::FIR(void)
    : taps_()
    , samples_()
    , head_(0)
{
}

template <typename T, typename T_TAPS>
void FIR<T,T_TAPS>::set_taps(const std::vector<T_TAPS>& taps)
{
    taps_.resize(taps.size());
    std::copy(taps.cbegin(), taps.cend(), taps_.begin());

    reset();
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
    head_ = 0;
}

template <typename T, typename T_TAPS>
void FIR<T,T_TAPS>::push(T x)
{
    samples_[head_++] = x;

    if (head_ == samples_.size()) {
        head_ = 0;
    }
}

template <typename T, typename T_TAPS>
T FIR<T,T_TAPS>::execute(void)
{
    T v = 0;

    for (std::size_t i = 0; i < taps_.size(); ++i) {
        v += taps_[i] * samples_[i];
    }

    return v;
}

template <typename T, typename T_TAPS>
std::vector<T> FIR<T,T_TAPS>::execute(const std::vector<T>& in)
{
    std::vector<T> out(in.size());

    execute(in, out);

    return out;
}

template <typename T, typename T_TAPS>
void FIR<T,T_TAPS>::execute(const std::vector<T>& in, std::vector<T>& out)
{
    out.resize(in.size());

    for (std::size_t i = 0; i < in.size(); ++i) {
        push(in[i]);
        out[i] = execute();
    }
}
