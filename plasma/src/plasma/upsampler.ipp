////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#include "kaiser.hpp"
#include <algorithm>

namespace plasma {

template <typename SampleType, typename FilterTapType>
upsampler<SampleType, FilterTapType>::upsampler(void) noexcept :
    num_filters_{1},
    num_taps_{0},
    coefficients_{},
    delay_line_re_{},
    delay_line_im_{},
    head_{0}
{
}

template <typename SampleType, typename FilterTapType>
upsampler<SampleType, FilterTapType>::upsampler(double in_sample_rate, double out_sample_rate) :
    upsampler{}
{
    set_rates(in_sample_rate, out_sample_rate);
}

template <typename SampleType, typename FilterTapType>
void upsampler<SampleType, FilterTapType>::set_rates(double in_sample_rate, double out_sample_rate)
{
    auto ratio = out_sample_rate / in_sample_rate;

    if (ratio < 1) {
        throw invalid_upsampling_ratio{};
    }

    if (std::abs(ratio - int(ratio)) > 0.001) {
        throw invalid_upsampling_ratio{};
    }

    num_filters_ = std::round(ratio);

    // generate filter taps
    //! \todo 2021-02-20: real roll-off factor and attenuation from variables
    auto kaiser_coeffs = kaiser::create_taps(0.5 / num_filters_, 0.1 / num_filters_, 60.0);

    // normalize coefficients
    auto max_coeff = *std::max_element(std::begin(kaiser_coeffs), std::end(kaiser_coeffs));

    for (auto& x : kaiser_coeffs) {
        x /= max_coeff;
    }

    // polyphase filters
    num_taps_ = std::ceil(kaiser_coeffs.size() / static_cast<double>(num_filters_));

    coefficients_.resize(num_filters_ * num_taps_);
    std::fill(std::begin(coefficients_), std::end(coefficients_), 0);

    for (unsigned int i = 0; i < kaiser_coeffs.size(); ++i) {
        auto filter_num = i % num_filters_;
        auto tap_num = i / num_filters_;

        coefficients_[filter_num * num_taps_ + tap_num] = kaiser_coeffs[i];
    }

    head_ = 0;

    delay_line_re_.resize(num_taps_);
    delay_line_im_.resize(num_taps_);

    std::fill(std::begin(delay_line_re_), std::end(delay_line_re_), 0);
    std::fill(std::begin(delay_line_im_), std::end(delay_line_im_), 0);
}

template <typename SampleType, typename FilterTapType>
void upsampler<SampleType, FilterTapType>::execute(std::vector<SampleType> const& in, std::vector<SampleType>& out) noexcept
{
    std::size_t len = in.size() / 2;
    out.resize(len * num_filters_ * 2);

    std::size_t out_idx = 0;

    // go through each input element
    for (std::size_t i = 0; i < len; ++i) {
        // add the element to the delay line
        delay_line_re_[head_] = in[i * 2];
        delay_line_im_[head_] = in[i * 2 + 1];

        // go through each polyphase filter
        for (unsigned int k = 0; k < num_filters_; ++k) {
            SampleType acc_re = 0;
            SampleType acc_im = 0;

            std::size_t read_idx = head_;
            auto filter_offset = k * num_taps_;

            // go through each coefficient
            for (unsigned int n = 0; n < num_taps_; ++n) {
                acc_re += delay_line_re_[read_idx] * coefficients_[filter_offset + num_taps_ - n - 1];
                acc_im += delay_line_im_[read_idx] * coefficients_[filter_offset + num_taps_ - n - 1];

                if (++read_idx == num_taps_) {
                    read_idx = 0;
                }
            }

            out[out_idx * 2] = acc_re;
            out[out_idx * 2 + 1] = acc_im;
            ++out_idx;
        }

        if (++head_ == num_taps_) {
            head_ = 0;
        }
    }
}

} // namespace
