////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_DSP_MEMORY_MEMORY_HPP_
#define PLASMA_DSP_MEMORY_MEMORY_HPP_

#include <cstdlib>
#include <memory>

namespace plasma::memory {

//! Allocate an array aligned to memory boundary
//! \tparam Alignment Alignment boundary in bytes
//! \tparam DataType Data type
//! \param[in] count Data count
//! \return unique pointer to allocated array
template <unsigned int Alignment, typename DataType>
auto aligned_array_alloc(unsigned int count)
{
    return std::unique_ptr<DataType, decltype(&std::free)>(
        reinterpret_cast<DataType*>(std::aligned_alloc(Alignment, sizeof(DataType) * count)),
        &std::free
    );
}

} // namespace

#endif
