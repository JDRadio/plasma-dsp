////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#include "math.hpp"
#include <cmath>

namespace plasma {

template <typename SampleType>
nco<SampleType, use_lookup>::nco(void) noexcept :
    theta_{0},
    dtheta_{0},
    lookup_{nullptr},
    cos_offset_{0},
    sin_offset_{0}
{
    build_lookup_table();
    set_frequency(0.0);
}

template <typename SampleType>
nco<SampleType, use_lookup>::~nco(void) noexcept
{
    delete[] lookup_;
}

template <typename SampleType>
void nco<SampleType, use_lookup>::set_frequency(double norm_freq) noexcept
{
    auto freq = norm_freq < 0 ? norm_freq + 1.0 : norm_freq;
    dtheta_ = static_cast<std::uint64_t>(std::round(freq * (1ULL << INTEGRAL_SIZE) * (1ULL<<MANTISSA_SIZE)));
}

template <typename SampleType>
void nco<SampleType, use_lookup>::build_lookup_table(void) noexcept
{
    std::uint64_t size = (1ULL << INTEGRAL_SIZE);
    cos_offset_ = (((static_cast<std::uint64_t>(size) >> 2)) << MANTISSA_SIZE) + (1ULL << (MANTISSA_SIZE-1));
    sin_offset_ = 1ULL << (MANTISSA_SIZE-1);
    lookup_ = new SampleType[size];

    auto const typed_size = static_cast<double>(size);

    for (unsigned int i = 0; i < size; ++i) {
        lookup_[i] = std::sin(TWO_PI * static_cast<double>(i) / typed_size);
    }
}

template <typename SampleType>
void nco<SampleType, use_lookup>::reset(void) noexcept
{
    dtheta_ = 0;
    theta_ = 0;
}

template <typename SampleType>
void nco<SampleType, use_lookup>::step(void) noexcept
{
    theta_ += dtheta_;
}


template <typename SampleType>
void nco<SampleType, use_lookup>::mix_up(std::vector<SampleType>& inout) noexcept
{
    auto const len = inout.size();
    auto const len_unrolled = static_cast<std::size_t>(len / 32) * 32;

    for (std::size_t i = 0; i < len_unrolled; i += 32) {
        mix_up_one(inout[i], inout[i+1]);
        mix_up_one(inout[i+2], inout[i+3]);
        mix_up_one(inout[i+4], inout[i+5]);
        mix_up_one(inout[i+6], inout[i+7]);

        mix_up_one(inout[i+8], inout[i+9]);
        mix_up_one(inout[i+10], inout[i+11]);
        mix_up_one(inout[i+12], inout[i+13]);
        mix_up_one(inout[i+14], inout[i+15]);

        mix_up_one(inout[i+16], inout[i+17]);
        mix_up_one(inout[i+18], inout[i+19]);
        mix_up_one(inout[i+20], inout[i+21]);
        mix_up_one(inout[i+22], inout[i+23]);

        mix_up_one(inout[i+24], inout[i+25]);
        mix_up_one(inout[i+26], inout[i+27]);
        mix_up_one(inout[i+28], inout[i+29]);
        mix_up_one(inout[i+30], inout[i+31]);
    }

    for (std::size_t i = len_unrolled; i < len; i += 2) {
        mix_up_one(inout[i], inout[i+1]);
    }
}

template <typename SampleType>
void nco<SampleType, use_lookup>::mix_up(std::vector<std::complex<SampleType>>& inout) noexcept
{
    auto const len = inout.size();
    auto const len_unrolled = static_cast<std::size_t>(len / 16) * 16;

    for (std::size_t i = 0; i < len_unrolled; i += 16) {
        mix_up_one(inout[i]);
        mix_up_one(inout[i+1]);
        mix_up_one(inout[i+2]);
        mix_up_one(inout[i+3]);

        mix_up_one(inout[i+4]);
        mix_up_one(inout[i+5]);
        mix_up_one(inout[i+6]);
        mix_up_one(inout[i+7]);

        mix_up_one(inout[i+8]);
        mix_up_one(inout[i+9]);
        mix_up_one(inout[i+10]);
        mix_up_one(inout[i+11]);

        mix_up_one(inout[i+12]);
        mix_up_one(inout[i+13]);
        mix_up_one(inout[i+14]);
        mix_up_one(inout[i+15]);
    }

    for (std::size_t i = len_unrolled; i < len; ++i) {
        mix_up_one(inout[i]);
    }
}


template <typename SampleType>
void nco<SampleType, use_lookup>::mix_down(std::vector<SampleType>& inout) noexcept
{
    auto const len = inout.size();
    auto const len_unrolled = static_cast<std::size_t>(len / 32) * 32;

    for (std::size_t i = 0; i < len_unrolled; i += 32) {
        mix_down_one(inout[i], inout[i+1]);
        mix_down_one(inout[i+2], inout[i+3]);
        mix_down_one(inout[i+4], inout[i+5]);
        mix_down_one(inout[i+6], inout[i+7]);

        mix_down_one(inout[i+8], inout[i+9]);
        mix_down_one(inout[i+10], inout[i+11]);
        mix_down_one(inout[i+12], inout[i+13]);
        mix_down_one(inout[i+14], inout[i+15]);

        mix_down_one(inout[i+16], inout[i+17]);
        mix_down_one(inout[i+18], inout[i+19]);
        mix_down_one(inout[i+20], inout[i+21]);
        mix_down_one(inout[i+22], inout[i+23]);

        mix_down_one(inout[i+24], inout[i+25]);
        mix_down_one(inout[i+26], inout[i+27]);
        mix_down_one(inout[i+28], inout[i+29]);
        mix_down_one(inout[i+30], inout[i+31]);
    }

    for (std::size_t i = len_unrolled; i < len; i += 2) {
        mix_down_one(inout[i], inout[i+1]);
    }
}

template <typename SampleType>
void nco<SampleType, use_lookup>::mix_down(std::vector<std::complex<SampleType>>& inout) noexcept
{
    auto const len = inout.size();
    auto const len_unrolled = static_cast<std::size_t>(len / 16) * 16;

    for (std::size_t i = 0; i < len_unrolled; i += 16) {
        mix_down_one(inout[i]);
        mix_down_one(inout[i+1]);
        mix_down_one(inout[i+2]);
        mix_down_one(inout[i+3]);

        mix_down_one(inout[i+4]);
        mix_down_one(inout[i+5]);
        mix_down_one(inout[i+6]);
        mix_down_one(inout[i+7]);

        mix_down_one(inout[i+8]);
        mix_down_one(inout[i+9]);
        mix_down_one(inout[i+10]);
        mix_down_one(inout[i+11]);

        mix_down_one(inout[i+12]);
        mix_down_one(inout[i+13]);
        mix_down_one(inout[i+14]);
        mix_down_one(inout[i+15]);
    }

    for (std::size_t i = len_unrolled; i < len; ++i) {
        mix_down_one(inout[i]);
    }
}


template <typename SampleType>
void nco<SampleType, use_lookup>::mix_up_one(SampleType& re, SampleType& im) noexcept
{
    auto const a = re;
    auto const b = im;
    auto const& c = lookup_[(theta_ + cos_offset_) >> MANTISSA_SIZE];
    auto const& d = lookup_[(theta_ + sin_offset_) >> MANTISSA_SIZE];

    re = a * c - b * d;
    im = a * d + b * c;

    theta_ += dtheta_;
}

template <typename SampleType>
void nco<SampleType, use_lookup>::mix_up_one(std::complex<SampleType>& inout) noexcept
{
    auto const a = inout.real();
    auto const b = inout.imag();
    auto const& c = lookup_[(theta_ + cos_offset_) >> MANTISSA_SIZE];
    auto const& d = lookup_[(theta_ + sin_offset_) >> MANTISSA_SIZE];

    inout.real(a * c - b * d);
    inout.imag(a * d + b * c);

    theta_ += dtheta_;
}


template <typename SampleType>
void nco<SampleType, use_lookup>::mix_down_one(SampleType& re, SampleType& im) noexcept
{
    auto const a = re;
    auto const b = im;
    auto const& c = lookup_[(theta_ + cos_offset_) >> MANTISSA_SIZE];
    auto const& d = lookup_[(theta_ + sin_offset_) >> MANTISSA_SIZE];

    re = a * c + b * d;
    im = b * c - a * d;

    theta_ += dtheta_;
}

template <typename SampleType>
void nco<SampleType, use_lookup>::mix_down_one(std::complex<SampleType>& inout) noexcept
{
    auto const a = inout.real();
    auto const b = inout.imag();
    auto const& c = lookup_[(theta_ + cos_offset_) >> MANTISSA_SIZE];
    auto const& d = lookup_[(theta_ + sin_offset_) >> MANTISSA_SIZE];

    inout.real(a * c + b * d);
    inout.imag(b * c - a * d);

    theta_ += dtheta_;
}

} // namespace
