////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_DSP_NCO_HPP_
#define PLASMA_DSP_NCO_HPP_

#include <vector>
#include <cstdint>
#include <type_traits>
#include <complex>

namespace plasma {

struct use_lookup {};
struct no_lookup {};

//! Numerically-Controlled Oscillator
template <typename SampleType, typename Option = no_lookup>
class nco
{
    static_assert(
        std::is_floating_point<SampleType>::value,
        "SampleType must be a floating point type"
    );

    static_assert(
        std::disjunction<std::is_same<Option, use_lookup>, std::is_same<Option, no_lookup>>::value,
        "Available options are: use_lookup, no_lookup"
    );

public:
    //! Default constructor
    nco(void) noexcept;

    //! Set the frequency
    //! \param[in] norm_freq Normalized frequency [-0.5, 0.5[
    void set_frequency(double norm_freq) noexcept;

    //! Mix up in place
    //! \param[in,out] inout Complex sample in pairs of real/imaginary components
    void mix_up(std::vector<SampleType>& inout) noexcept;

    //! Mix up in place
    //! \param[in,out] inout Complex samples
    void mix_up(std::vector<std::complex<SampleType>>& inout) noexcept;


    //! Mix up a single sample
    //! \param[in,out] re Real part
    //! \param[in,out] im Imaginary part
    void mix_up_one(SampleType& re, SampleType& im) noexcept;

    //! Mix up a single sample
    //! \param[in,out] inout Complex sample
    void mix_up_one(std::complex<SampleType>& inout) noexcept;


    //! Mix down in place
    //! \param[in,out] inout Complex samples in pairs of real/imaginary components
    void mix_down(std::vector<SampleType>& inout) noexcept;

    //! Mix down in place
    //! \param[in,out] inout Complex samples
    void mix_down(std::vector<std::complex<SampleType>>& inout) noexcept;


    //! Mix down a single sample
    //! \param[in,out] re Real part
    //! \param[in,out] im Imaginary part
    void mix_down_one(SampleType& re, SampleType& im) noexcept;

    //! Mix down a single sample
    //! \param[in,out] inout Complex sample
    void mix_down_one(std::complex<SampleType>& inout) noexcept;


    //! Reset the NCO phase and frequency
    void reset(void) noexcept;

    //! Step forward by one sample
    //! \note You do not need to call this after calling any of the mixing functions
    void step(void) noexcept;
};


template <typename SampleType>
class nco<SampleType, use_lookup>
{
public:
    //! Default constructor
    nco(void) noexcept;

    //! Destructor
    ~nco(void) noexcept;

    //! Set the frequency
    //! \param[in] norm_freq Normalized frequency [-0.5, 0.5[
    void set_frequency(double norm_freq) noexcept;

    //! Mix up in place
    //! \param[in,out] inout Complex sample in pairs of real/imaginary components
    void mix_up(std::vector<SampleType>& inout) noexcept;

    //! Mix up in place
    //! \param[in,out] inout Complex samples
    void mix_up(std::vector<std::complex<SampleType>>& inout) noexcept;


    //! Mix up a single sample
    //! \param[in,out] re Real part
    //! \param[in,out] im Imaginary part
    void mix_up_one(SampleType& re, SampleType& im) noexcept;

    //! Mix up a single sample
    //! \param[in,out] inout Complex sample
    void mix_up_one(std::complex<SampleType>& inout) noexcept;


    //! Mix down in place
    //! \param[in,out] inout Complex samples in pairs of real/imaginary components
    void mix_down(std::vector<SampleType>& inout) noexcept;

    //! Mix down in place
    //! \param[in,out] inout Complex samples
    void mix_down(std::vector<std::complex<SampleType>>& inout) noexcept;


    //! Mix down a single sample
    //! \param[in,out] re Real part
    //! \param[in,out] im Imaginary part
    void mix_down_one(SampleType& re, SampleType& im) noexcept;

    //! Mix down a single sample
    //! \param[in,out] inout Complex sample
    void mix_down_one(std::complex<SampleType>& inout) noexcept;


    //! Reset the NCO phase and frequency
    void reset(void) noexcept;

    //! Step forward by one sample
    //! \note You do not need to call this after calling any of the mixing functions
    void step(void) noexcept;

public:
    //! Integral part size in bits
    static constexpr auto INTEGRAL_SIZE = 16;
    //! Fractional part size in bits
    static constexpr auto MANTISSA_SIZE = 64 - INTEGRAL_SIZE;


    //! Build the lookup table
    void build_lookup_table(void) noexcept;


    //! Phase
    std::uint64_t theta_;
    //! Frequency
    std::uint64_t dtheta_;
    //! Lookup table
    SampleType* lookup_;
    //! Cosine offset
    std::uint64_t cos_offset_;
    //! Sine offset
    std::uint64_t sin_offset_;
};


template <typename SampleType>
class nco<SampleType, no_lookup>
{
public:
    //! Default constructor
    nco(void) noexcept;

    //! Set the frequency
    //! \param[in] norm_freq Normalized frequency [-0.5, 0.5[
    void set_frequency(double norm_freq) noexcept;

    //! Mix up in place
    //! \param[in,out] inout Complex sample in pairs of real/imaginary components
    void mix_up(std::vector<SampleType>& inout) noexcept;

    //! Mix up in place
    //! \param[in,out] inout Complex samples
    void mix_up(std::vector<std::complex<SampleType>>& inout) noexcept;


    //! Mix up a single sample
    //! \param[in,out] re Real part
    //! \param[in,out] im Imaginary part
    void mix_up_one(SampleType& re, SampleType& im) noexcept;

    //! Mix up a single sample
    //! \param[in,out] inout Complex sample
    void mix_up_one(std::complex<SampleType>& inout) noexcept;


    //! Mix down in place
    //! \param[in,out] inout Complex samples in pairs of real/imaginary components
    void mix_down(std::vector<SampleType>& inout) noexcept;

    //! Mix down in place
    //! \param[in,out] inout Complex samples
    void mix_down(std::vector<std::complex<SampleType>>& inout) noexcept;


    //! Mix down a single sample
    //! \param[in,out] re Real part
    //! \param[in,out] im Imaginary part
    void mix_down_one(SampleType& re, SampleType& im) noexcept;

    //! Mix down a single sample
    //! \param[in,out] inout Complex sample
    void mix_down_one(std::complex<SampleType>& inout) noexcept;


    //! Reset the NCO phase and frequency
    void reset(void) noexcept;

    //! Step forward by one sample
    //! \note You do not need to call this after calling any of the mixing functions
    void step(void) noexcept;


    //! Set the PLL bandwidth
    //! \param[in] bandwidth Bandwidth
    void set_pll_bandwidth(double bandwidth) noexcept;

    //! Step the PLL
    //! \param[in] dphi Angular correction
    void pll_step(double dphi) noexcept;

private:
    //! Phase
    double theta_;
    //! Frequency
    double dtheta_;

    //! PLL feedback parameter for frequency
    double alpha_;
    //! PLL feedback parameter for phase
    double beta_;
};

} // namespace

#include "nco_no_lookup.ipp"
#include "nco_use_lookup.ipp"

#endif
