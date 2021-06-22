////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_DSP_FILTER_FILTER_HPP_
#define PLASMA_DSP_FILTER_FILTER_HPP_

#include "plasma/memory/memory.hpp"
#include <vector>
#include <memory>
#include <cstdlib>
#include <cstring>
#include <xmmintrin.h>
#include <immintrin.h>
#include <pmmintrin.h>
#include <emmintrin.h>

namespace plasma {

class filter
{
public:
    filter(void) noexcept :
        num_taps_{0},
        aligned_taps_{0},
        coefficients_ptr_{nullptr, &std::free},
        delay_line_i_ptr_{nullptr, &std::free},
        delay_line_q_ptr_{nullptr, &std::free},
        coefficients_{nullptr},
        delay_line_i_{nullptr},
        delay_line_q_{nullptr}
    {
    }

    filter(std::vector<float> const& taps) noexcept :
        filter{}
    {
        prepare(taps.size(), taps.data());
    }

    filter(unsigned int num_taps, float const* taps) noexcept :
        filter{}
    {
        prepare(num_taps, taps);
    }

    void reset(void) noexcept
    {
        std::fill_n(delay_line_i_, aligned_taps_, 0);
        std::fill_n(delay_line_q_, aligned_taps_, 0);
    }

    static inline float sum4(__m128 x)
    {
        x = _mm_hadd_ps(x, x);
        x = _mm_hadd_ps(x, x);

        return _mm_cvtss_f32(x);
    }

    static inline float sum8(__m256 x)
    {
        return sum4(_mm_add_ps(_mm256_castps256_ps128(x), _mm256_extractf128_ps(x, 1)));
    }

    void push(float const* in) noexcept
    {
        delay_line_i_[num_taps_ - 1] = in[0];
        delay_line_q_[num_taps_ - 1] = in[1];
    }

    void linearize(void) noexcept
    {
        std::memmove(delay_line_i_, delay_line_i_ + 1, (num_taps_ - 1) * sizeof(float));
        std::memmove(delay_line_q_, delay_line_q_ + 1, (num_taps_ - 1) * sizeof(float));
    }

    std::vector<float> execute(std::vector<float> const& in) noexcept
    {
        auto const size = in.size();
        std::vector<float> out(size);
        execute(size, in.data(), out.data());
        return out;
    }

    void execute(unsigned int num, float const* in, float* out) noexcept
    {
        for (unsigned int n = 0; n < num; n += 2) {
            linearize();
            push(in + n);
            extract(out + n);
        }
    }

    void extract(float* out) noexcept
    {
        unsigned int n = 0;

        __m256 x_acc_i{0};
        __m256 x_acc_q{0};

        while (n < aligned_taps_) {
            __m256 x_delay_i = _mm256_load_ps(delay_line_i_ + n);
            __m256 x_delay_q = _mm256_load_ps(delay_line_q_ + n);
            __m256 x_coeff = _mm256_load_ps(coefficients_ + n);

            x_acc_i = _mm256_fmadd_ps(x_delay_i, x_coeff, x_acc_i);
            x_acc_q = _mm256_fmadd_ps(x_delay_q, x_coeff, x_acc_q);

            n += 8;
        }

        out[0] = sum8(x_acc_i);
        out[1] = sum8(x_acc_q);
    }

private:
    void prepare(unsigned int num_taps, float const* taps) noexcept
    {
        num_taps_ = num_taps;
        aligned_taps_ = ((num_taps_ >> 3) + ((num_taps_ % 8) ? 1 : 0)) << 3;

        // get 32-byte aligned buffers
        coefficients_ptr_ = memory::aligned_array_alloc<32, float>(aligned_taps_);
        delay_line_i_ptr_ = memory::aligned_array_alloc<32, float>(aligned_taps_ + 1);
        delay_line_q_ptr_ = memory::aligned_array_alloc<32, float>(aligned_taps_ + 1);

        coefficients_ = coefficients_ptr_.get();
        delay_line_i_ = delay_line_i_ptr_.get();
        delay_line_q_ = delay_line_q_ptr_.get();

        std::fill_n(coefficients_, aligned_taps_, 0);
        std::fill_n(delay_line_i_, aligned_taps_ + 1, 0);
        std::fill_n(delay_line_q_, aligned_taps_ + 1, 0);

        for (unsigned int i = 0; i < num_taps; ++i) {
            coefficients_[num_taps_ - i - 1] = taps[i];
        }
    }


    unsigned int num_taps_;
    //! Number of taps per filter, aligned to 16 elements
    unsigned int aligned_taps_;
    //! Filter coefficients
    std::unique_ptr<float, decltype(&std::free)> coefficients_ptr_;
    //! Delay line - Real component
    std::unique_ptr<float, decltype(&std::free)> delay_line_i_ptr_;
    //! Delay line - Imaginary component
    std::unique_ptr<float, decltype(&std::free)> delay_line_q_ptr_;
    //! Filter coefficients
    float* coefficients_;
    //! Delay line - Real component
    float* delay_line_i_;
    //! Delay line - Imaginary component
    float* delay_line_q_;
};

} // namespace

#endif
