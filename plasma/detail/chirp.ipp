////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the AmateurRadio.Engineer project
//! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
//! \date 2020
//! \copyright AmateurRadio.Engineer
////////////////////////////////////////////////////////////////////////////////
template <typename T>
CHIRP<T>::CHIRP(void)
    : sample_rate_(0)
    , freq_start_(0)
    , freq_end_(0)
    , freq_adjust_(0)
    , sweep_time_(0)
    , nco_()
{
}

template <typename T>
std::complex<T> CHIRP<T>::execute(void)
{
    return nco_.mix_up(std::complex<T>(1, 0));
}

template <typename T>
std::vector<std::complex<T>> CHIRP<T>::execute(std::size_t count)
{
    std::vector<std::complex<T>> out(count);

    for (std::size_t i = 0; i < count; i++) {
        out[i] = this->execute();
        this->step();
    }

    return out;
}

template <typename T>
void CHIRP<T>::step(void)
{
    nco_.adjust_frequency(freq_adjust_);

    if (nco_.get_frequency() >= freq_end_) {
        nco_.set_frequency(freq_start_);
    }

    nco_.step();
}

template <typename T>
void CHIRP<T>::set_parameters(double sample_rate, double start, double end, double time)
{
    sample_rate_ = sample_rate;
    freq_start_ = start / sample_rate_;
    freq_end_ = end / sample_rate_;
    sweep_time_ = time;

    freq_adjust_ = (freq_end_ - freq_start_) / (sweep_time_ * sample_rate_);
    nco_.set_frequency(freq_start_);
}
