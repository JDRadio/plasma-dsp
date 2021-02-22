////////////////////////////////////////////////////////////////////////////////
//! \file
//! \details This file is part of the plasma-dsp project
//! \author Jean-Sebastien Dominique <jd@jdradio.dev>
//! \date 2021
//! \copyright JDRadio Inc.
////////////////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>
#include <plasma/nco.hpp>
#include <iostream>
#include <iomanip>

TEST(nco, use_lookup_float)
{
    plasma::nco<float, plasma::use_lookup> nco;
    nco.set_frequency(0.001214);

    plasma::nco<float, plasma::no_lookup> nco_ref;
    nco_ref.set_frequency(0.001214);

    std::vector<float> vec(10240 * 2, 0.01);
    std::vector<float> vec_ref(10240 * 2, 0.01);

    nco.mix_up(vec);
    nco_ref.mix_up(vec_ref);

    static constexpr auto precision = 1e-6;

    for (std::size_t i = 0; i < 20480; i += 2) {
        ASSERT_NEAR(vec[i], vec_ref[i], precision);
        ASSERT_NEAR(vec[i+1], vec_ref[i+1], precision);
    }
}

TEST(nco, use_lookup_double)
{
    plasma::nco<double, plasma::use_lookup> nco;
    nco.set_frequency(0.001214);

    plasma::nco<double, plasma::no_lookup> nco_ref;
    nco_ref.set_frequency(0.001214);

    std::vector<double> vec(10240 * 2, 0.01);
    std::vector<double> vec_ref(10240 * 2, 0.01);

    nco.mix_up(vec);
    nco_ref.mix_up(vec_ref);

    static constexpr auto precision = 1e-6;

    for (std::size_t i = 0; i < 20480; i += 2) {
        EXPECT_NEAR(vec[i], vec_ref[i], precision);
        EXPECT_NEAR(vec[i+1], vec_ref[i+1], precision);
    }
}

TEST(nco, no_lookup_float)
{
    plasma::nco<float, plasma::no_lookup> nco;
    nco.set_frequency(0.01);

    std::vector<float> vec(22 * 2, 0.01);

    nco.mix_up(vec);

    static constexpr auto precision = 1e-9;

    ASSERT_NEAR(vec[0], 0.01, precision);
    ASSERT_NEAR(vec[1], 0.01, precision);

    ASSERT_NEAR(vec[2], 0.009352362089, precision);
    ASSERT_NEAR(vec[3], 0.01060817248, precision);

    ASSERT_NEAR(vec[4], 0.008667814678, precision);
    ASSERT_NEAR(vec[5], 0.01117447935, precision);

    ASSERT_NEAR(vec[6], 0.007949059361, precision);
    ASSERT_NEAR(vec[7], 0.01169668565, precision);

    ASSERT_NEAR(vec[8], 0.007198932740, precision);
    ASSERT_NEAR(vec[9], 0.01217273048, precision);

    ASSERT_NEAR(vec[10], 0.006420395219, precision);
    ASSERT_NEAR(vec[11], 0.01260073511, precision);

    ASSERT_NEAR(vec[12], 0.005616519332, precision);
    ASSERT_NEAR(vec[13], 0.01297901039, precision);

    ASSERT_NEAR(vec[14], 0.004790477609, precision);
    ASSERT_NEAR(vec[15], 0.01330606344, precision);

    ASSERT_NEAR(vec[16], 0.003945530059, precision);
    ASSERT_NEAR(vec[17], 0.01358060354, precision);

    ASSERT_NEAR(vec[18], 0.003085011305, precision);
    ASSERT_NEAR(vec[19], 0.01380154720, precision);

    ASSERT_NEAR(vec[20], 0.002212317421, precision);
    ASSERT_NEAR(vec[21], 0.01396802247, precision);
}

TEST(nco, no_lookup_double)
{
    plasma::nco<double, plasma::no_lookup> nco;
    nco.set_frequency(0.01);

    std::vector<double> vec(22 * 2, 0.01);

    nco.mix_up(vec);

    static constexpr auto precision = 1e-9;

    ASSERT_NEAR(vec[0], 0.01, precision);
    ASSERT_NEAR(vec[1], 0.01, precision);

    ASSERT_NEAR(vec[2], 0.009352362089, precision);
    ASSERT_NEAR(vec[3], 0.01060817248, precision);

    ASSERT_NEAR(vec[4], 0.008667814678, precision);
    ASSERT_NEAR(vec[5], 0.01117447935, precision);

    ASSERT_NEAR(vec[6], 0.007949059361, precision);
    ASSERT_NEAR(vec[7], 0.01169668565, precision);

    ASSERT_NEAR(vec[8], 0.007198932740, precision);
    ASSERT_NEAR(vec[9], 0.01217273048, precision);

    ASSERT_NEAR(vec[10], 0.006420395219, precision);
    ASSERT_NEAR(vec[11], 0.01260073511, precision);

    ASSERT_NEAR(vec[12], 0.005616519332, precision);
    ASSERT_NEAR(vec[13], 0.01297901039, precision);

    ASSERT_NEAR(vec[14], 0.004790477609, precision);
    ASSERT_NEAR(vec[15], 0.01330606344, precision);

    ASSERT_NEAR(vec[16], 0.003945530059, precision);
    ASSERT_NEAR(vec[17], 0.01358060354, precision);

    ASSERT_NEAR(vec[18], 0.003085011305, precision);
    ASSERT_NEAR(vec[19], 0.01380154720, precision);

    ASSERT_NEAR(vec[20], 0.002212317421, precision);
    ASSERT_NEAR(vec[21], 0.01396802247, precision);
}
