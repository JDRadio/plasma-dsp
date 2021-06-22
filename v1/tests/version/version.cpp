////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>
#include "plasma/version/version.hpp"

namespace {

TEST(Version, GetCompiled)
{
    auto const major = plasma::version::version::get_major();
    auto const minor = plasma::version::version::get_minor();
    auto const revision = plasma::version::version::get_revision();
    auto const number = plasma::version::version::get();

    EXPECT_EQ(major, plasma::version::version::MAJOR);
    EXPECT_EQ(minor, plasma::version::version::MINOR);
    EXPECT_EQ(revision, plasma::version::version::REVISION);
    EXPECT_EQ(number, plasma::version::version::VERSION_NUMBER);
}

} // namespace
