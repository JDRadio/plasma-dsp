////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#ifndef PLASMA_DSP_VERSION_HPP_
#define PLASMA_DSP_VERSION_HPP_

namespace plasma::version {

class version
{
    version(void) noexcept = delete;
    ~version(void) noexcept = delete;

public:
    //! Get the compiled major version
    static unsigned int get_major(void) noexcept;

    //! Get the compiled minor version
    static unsigned int get_minor(void) noexcept;

    //! Get the compiled revision
    static unsigned int get_revision(void) noexcept;

    //! Get the compiled version as a comparable number in the format MMmmrrrr
    static unsigned int get(void) noexcept;


    //! Major version
    static constexpr unsigned int MAJOR = 0;
    //! Minor version
    static constexpr unsigned int MINOR = 1;
    //! Revision
    static constexpr unsigned int REVISION = 0;

    //! Version number in the format MMmmrrrr
    static constexpr unsigned int VERSION_NUMBER = (MAJOR * 1000000) + (MINOR * 10000) + REVISION;
};

} // namespace

#endif
