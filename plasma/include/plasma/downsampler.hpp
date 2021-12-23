////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_DSP_DOWNSAMPLER_HPP_
#define PLASMA_DSP_DOWNSAMPLER_HPP_

#include <vector>
#include <memory>
#include <complex>
#include <exception>

namespace plasma {

//! Invalid downsampling ratio
class invalid_downsampling_ratio : public std::exception
{
public:
    //! \copydoc std::exception::what()
    const char* what(void) const noexcept override { return "invalid downsampling ratio"; }
};

//! Downsampler
//!
//! Convert a signal from a sample rate to a different lower one, only using integer increments
template <typename SampleType, typename FilterTapType>
class downsampler
{
public:
    //! Default constructor
    downsampler(void) noexcept;

    //! Constructor
    //! \param[in] in_sample_rate Input sample rate
    //! \param[in] out_sample_rate Output sample rate
    //! \throws invalid_ratio_upsample_only
    downsampler(double in_sample_rate, double out_sample_rate);

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
    //! Sample number
    unsigned int head_;
    //! Delay line write index
    unsigned int tap_index_;
};

} // namespace

#include "downsampler.ipp"

#endif
