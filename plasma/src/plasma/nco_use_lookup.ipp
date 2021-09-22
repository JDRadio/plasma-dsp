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
        lookup_[i] = std::sin(math::TWO_PI * static_cast<double>(i) / typed_size);
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
    auto* ptr = inout.data();
    unsigned int const len = inout.size();
    unsigned int const len_unrolled = static_cast<unsigned int>(len / 32) * 32;

    for (unsigned int i = 0; i < len_unrolled; i += 32) {
        mix_up_one(ptr[i], ptr[i+1]);
        mix_up_one(ptr[i+2], ptr[i+3]);
        mix_up_one(ptr[i+4], ptr[i+5]);
        mix_up_one(ptr[i+6], ptr[i+7]);

        mix_up_one(ptr[i+8], ptr[i+9]);
        mix_up_one(ptr[i+10], ptr[i+11]);
        mix_up_one(ptr[i+12], ptr[i+13]);
        mix_up_one(ptr[i+14], ptr[i+15]);

        mix_up_one(ptr[i+16], ptr[i+17]);
        mix_up_one(ptr[i+18], ptr[i+19]);
        mix_up_one(ptr[i+20], ptr[i+21]);
        mix_up_one(ptr[i+22], ptr[i+23]);

        mix_up_one(ptr[i+24], ptr[i+25]);
        mix_up_one(ptr[i+26], ptr[i+27]);
        mix_up_one(ptr[i+28], ptr[i+29]);
        mix_up_one(ptr[i+30], ptr[i+31]);
    }

    for (unsigned int i = len_unrolled; i < len; i += 2) {
        mix_up_one(ptr[i], ptr[i+1]);
    }
}

template <typename SampleType>
void nco<SampleType, use_lookup>::mix_up(std::vector<std::complex<SampleType>>& inout) noexcept
{
    auto* ptr = inout.data();
    unsigned int const len = inout.size();
    unsigned int const len_unrolled = static_cast<unsigned int>(len / 16) * 16;

    for (unsigned int i = 0; i < len_unrolled; i += 16) {
        mix_up_one(*ptr++);
        mix_up_one(*ptr++);
        mix_up_one(*ptr++);
        mix_up_one(*ptr++);

        mix_up_one(*ptr++);
        mix_up_one(*ptr++);
        mix_up_one(*ptr++);
        mix_up_one(*ptr++);

        mix_up_one(*ptr++);
        mix_up_one(*ptr++);
        mix_up_one(*ptr++);
        mix_up_one(*ptr++);

        mix_up_one(*ptr++);
        mix_up_one(*ptr++);
        mix_up_one(*ptr++);
        mix_up_one(*ptr++);
    }

    for (unsigned int i = len_unrolled; i < len; ++i) {
        mix_up_one(*ptr++);
    }
}


template <typename SampleType>
void nco<SampleType, use_lookup>::mix_down(std::vector<SampleType>& inout) noexcept
{
    auto* ptr = inout.data();
    unsigned int const len = inout.size();
    unsigned int const len_unrolled = static_cast<unsigned int>(len / 32) * 32;

    for (unsigned int i = 0; i < len_unrolled; i += 32) {
        mix_down_one(ptr[i], ptr[i+1]);
        mix_down_one(ptr[i+2], ptr[i+3]);
        mix_down_one(ptr[i+4], ptr[i+5]);
        mix_down_one(ptr[i+6], ptr[i+7]);

        mix_down_one(ptr[i+8], ptr[i+9]);
        mix_down_one(ptr[i+10], ptr[i+11]);
        mix_down_one(ptr[i+12], ptr[i+13]);
        mix_down_one(ptr[i+14], ptr[i+15]);

        mix_down_one(ptr[i+16], ptr[i+17]);
        mix_down_one(ptr[i+18], ptr[i+19]);
        mix_down_one(ptr[i+20], ptr[i+21]);
        mix_down_one(ptr[i+22], ptr[i+23]);

        mix_down_one(ptr[i+24], ptr[i+25]);
        mix_down_one(ptr[i+26], ptr[i+27]);
        mix_down_one(ptr[i+28], ptr[i+29]);
        mix_down_one(ptr[i+30], ptr[i+31]);
    }

    for (unsigned int i = len_unrolled; i < len; i += 2) {
        mix_down_one(ptr[i], ptr[i+1]);
    }
}

template <typename SampleType>
void nco<SampleType, use_lookup>::mix_down(std::vector<std::complex<SampleType>>& inout) noexcept
{
    auto* ptr = inout.data();
    unsigned int const len = inout.size();
    unsigned int const len_unrolled = static_cast<unsigned int>(len / 16) * 16;

    for (unsigned int i = 0; i < len_unrolled; i += 16) {
        mix_down_one(*ptr++);
        mix_down_one(*ptr++);
        mix_down_one(*ptr++);
        mix_down_one(*ptr++);

        mix_down_one(*ptr++);
        mix_down_one(*ptr++);
        mix_down_one(*ptr++);
        mix_down_one(*ptr++);

        mix_down_one(*ptr++);
        mix_down_one(*ptr++);
        mix_down_one(*ptr++);
        mix_down_one(*ptr++);

        mix_down_one(*ptr++);
        mix_down_one(*ptr++);
        mix_down_one(*ptr++);
        mix_down_one(*ptr++);
    }

    for (unsigned int i = len_unrolled; i < len; ++i) {
        mix_down_one(*ptr++);
    }
}


template <typename SampleType>
void nco<SampleType, use_lookup>::mix_up_one(SampleType& re, SampleType& im) noexcept
{
    auto const a = re;
    auto const b = im;
    auto const c = lookup_[(theta_ + cos_offset_) >> MANTISSA_SIZE];
    auto const d = lookup_[(theta_ + sin_offset_) >> MANTISSA_SIZE];

    re = a * c - b * d;
    im = a * d + b * c;

    theta_ += dtheta_;
}

template <typename SampleType>
void nco<SampleType, use_lookup>::mix_up_one(std::complex<SampleType>& inout) noexcept
{
    auto const a = inout.real();
    auto const b = inout.imag();
    auto const c = lookup_[(theta_ + cos_offset_) >> MANTISSA_SIZE];
    auto const d = lookup_[(theta_ + sin_offset_) >> MANTISSA_SIZE];

    inout.real(a * c - b * d);
    inout.imag(a * d + b * c);

    theta_ += dtheta_;
}


template <typename SampleType>
void nco<SampleType, use_lookup>::mix_down_one(SampleType& re, SampleType& im) noexcept
{
    auto const a = re;
    auto const b = im;
    auto const c = lookup_[(theta_ + cos_offset_) >> MANTISSA_SIZE];
    auto const d = lookup_[(theta_ + sin_offset_) >> MANTISSA_SIZE];

    re = a * c + b * d;
    im = b * c - a * d;

    theta_ += dtheta_;
}

template <typename SampleType>
void nco<SampleType, use_lookup>::mix_down_one(std::complex<SampleType>& inout) noexcept
{
    auto const a = inout.real();
    auto const b = inout.imag();
    auto const c = lookup_[(theta_ + cos_offset_) >> MANTISSA_SIZE];
    auto const d = lookup_[(theta_ + sin_offset_) >> MANTISSA_SIZE];

    inout.real(a * c + b * d);
    inout.imag(b * c - a * d);

    theta_ += dtheta_;
}

} // namespace
