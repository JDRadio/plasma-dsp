////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the AmateurRadio.Engineer project
//! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
//! \date 2020
//! \copyright AmateurRadio.Engineer
////////////////////////////////////////////////////////////////////////////////
template <typename T, typename TC>
HB_DOWNSAMPLER<T,TC>::HB_DOWNSAMPLER(void)
    : samples_(0)
    , filter_()
{
}

template <typename T, typename TC>
void HB_DOWNSAMPLER<T,TC>::set_length(std::size_t length)
{
    //! \todo 2020-06-21: Generate the half-band filter taps
    // filter_.set_taps(taps);
}

template <typename T, typename TC>
void HB_DOWNSAMPLER<T,TC>::reset(void)
{
    samples_ = 0;
    filter_.reset();
}

template <typename T, typename TC>
void HB_DOWNSAMPLER<T,TC>::execute(const std::vector<TC>& in, std::vector<TC>& out)
{
    //! \todo 2020-06-21: Optimize operations due to filter having a lot of zeros
    out.reserve(in.size() / 2 + 1);

    for (std::size_t i = 0; i < in.size(); ++i) {
        filter_.push(in[i]);

        if (++samples_ % 2 == 0) {
            out.push_back(filter_.execute());
            samples_ = 0;
        }
    }
}
