////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#include "kaiser.hpp"
#include <algorithm>
#include <numeric>

namespace plasma {

template <typename SampleType, typename FilterTapType>
downsampler<SampleType, FilterTapType>::downsampler(void) noexcept :
    num_filters_{1},
    num_taps_{0},
    coefficients_{},
    delay_line_re_{},
    delay_line_im_{},
    head_{0},
    tap_index_{0}
{
}

template <typename SampleType, typename FilterTapType>
downsampler<SampleType, FilterTapType>::downsampler(double in_sample_rate, double out_sample_rate) :
    downsampler{}
{
    set_rates(in_sample_rate, out_sample_rate);
}

template <typename SampleType, typename FilterTapType>
void downsampler<SampleType, FilterTapType>::set_rates(double in_sample_rate, double out_sample_rate)
{
    auto ratio = in_sample_rate / out_sample_rate;

    if (ratio < 1) {
        throw invalid_downsampling_ratio{};
    }

    if (std::abs(ratio - int(ratio)) > 0.001) {
        throw invalid_downsampling_ratio{};
    }

    num_filters_ = std::round(ratio);

    // generate filter taps
    //! \todo 2021-02-20: read filter roll-off factor and attenuation from variables
    coefficients_ = kaiser::create_taps(0.5 / ratio, 0.25 / ratio, 60.0);

    // normalize coefficients
    auto max_coeff = *std::max_element(std::begin(coefficients_), std::end(coefficients_));

    for (auto& x : coefficients_) {
        x /= max_coeff;
    }

    num_taps_ = coefficients_.size();

    head_ = 0;
    tap_index_ = 0;

    delay_line_re_.resize(num_taps_);
    delay_line_im_.resize(num_taps_);

    std::fill(std::begin(delay_line_re_), std::end(delay_line_re_), 0);
    std::fill(std::begin(delay_line_im_), std::end(delay_line_im_), 0);
}

template <typename SampleType, typename FilterTapType>
void downsampler<SampleType, FilterTapType>::execute(std::vector<SampleType> const& in, std::vector<SampleType>& out) noexcept
{
    std::size_t len = in.size() / 2;
    out.resize((len / num_filters_ + 1) * 2);

    std::size_t out_idx = 0;

    // go through each input element
    for (std::size_t i = 0; i < len; ++i) {
        delay_line_re_[tap_index_] = in[i * 2];
        delay_line_im_[tap_index_] = in[i * 2 + 1];

        // keep 1 in D samples
        if (++head_ == num_filters_) {
            head_ = 0;

            SampleType acc_re = 0;
            SampleType acc_im = 0;

            std::size_t read_idx = tap_index_;

            // go through each coefficient
            for (unsigned int n = 0; n < num_taps_; ++n) {
                acc_re += delay_line_re_[read_idx] * coefficients_[num_taps_ - n - 1];
                acc_im += delay_line_im_[read_idx] * coefficients_[num_taps_ - n - 1];

                if (++read_idx == num_taps_) {
                    read_idx = 0;
                }
            }

            out[out_idx * 2] = acc_re / num_filters_;
            out[out_idx * 2 + 1] = acc_im / num_filters_;
            ++out_idx;
        }

        if (++tap_index_ == num_taps_) {
            tap_index_ = 0;
        }
    }

    out.resize(out_idx * 2);
}

} // namespace
