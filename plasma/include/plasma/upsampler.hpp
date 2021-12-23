////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_DSP_UPSAMPLER_HPP_
#define PLASMA_DSP_UPSAMPLER_HPP_

#include <vector>
#include <memory>
#include <complex>
#include <exception>

namespace plasma {

//! Invalid upsampling ratio
class invalid_upsampling_ratio : public std::exception
{
public:
    //! \copydoc std::exception::what()
    const char* what(void) const noexcept override { return "invalid upsampling ratio"; }
};


//! Upsampler
//!
//! Convert a signal from a sample rate to a different higher one, only using integer increments
template <typename SampleType, typename FilterTapType>
class upsampler
{
public:

    //! Default constructor
    upsampler(void) noexcept;

    //! Constructor
    //! \param[in] in_sample_rate Input sample rate
    //! \param[in] out_sample_rate Output sample rate
    //! \throws invalid_ratio_upsample_only
    upsampler(double in_sample_rate, double out_sample_rate);

    //! Set sampling rates
    //! \param[in] in_sample_rate Input sample rate
    //! \param[in] out_sample_rate Output sample rate
    //! \throws invalid_ratio_upsample_only
    void set_rates(double in_sample_rate, double out_sample_rate);

    //! Process the data
    //! \param[in] in Input
    //! \param[out] out Output
    void execute(std::vector<SampleType> const& in, std::vector<SampleType>& out) noexcept;

private:
    //! Number of filters
    unsigned int num_filters_;
    //! Number of taps per filter
    unsigned int num_taps_;
    //! Filter coefficients
    std::vector<FilterTapType> coefficients_;
    //! Delay line - Real component
    std::vector<SampleType> delay_line_re_;
    //! Delay line - Imaginary component
    std::vector<SampleType> delay_line_im_;
    //! Delay line write head
    unsigned int head_;
};

} // namespace

#include "upsampler.ipp"

#endif
