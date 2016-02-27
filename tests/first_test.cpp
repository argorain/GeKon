//
// Created by sehny on 2/26/16.
//

#include "gtest/gtest.h"

#include <cmath>
#include <opencv2/core.hpp>
#include "gk_types.h"
#include "gk_functions.h"

TEST(basic_check, test_eq) {
    EXPECT_EQ(1,0);
}

TEST(basic_check, test_neq) {
    EXPECT_NE(1,0);
}

TEST(fitness_mse_test, test_equal) {
    cv::Mat kernel = cv::Mat::ones(3,3,CV_8U);
    cv::Mat input = (cv::Mat_<unsigned char>(3,3) << 2,3,1,4,5,6,1,2,3);
    cv::Mat output = (cv::Mat_<unsigned char>(3,3) << 16,23,17,19,29,22,14,23,18);
    double expected_mse = 2;

    gekon::tr_sample_t sample = {
            .original = input,
            .modified = output
    };
    gekon::candidate_t cand = kernel;

    double mse = gekon::fitness_mse(sample, cand);

    EXPECT_LE(abs(mse-expected_mse), std::numeric_limits<double>::epsilon());
}
