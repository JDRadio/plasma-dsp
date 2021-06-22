////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#include "plasma/version/version.hpp"

namespace plasma::version {

unsigned int version::get_major(void) noexcept
{
    return MAJOR;
}

unsigned int version::get_minor(void) noexcept
{
    return MINOR;
}

unsigned int version::get_revision(void) noexcept
{
    return REVISION;
}

unsigned int version::get(void) noexcept
{
    return VERSION_NUMBER;
}

} // namespace
