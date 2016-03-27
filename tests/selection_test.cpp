//
// Created by sehny on 3/27/16.
//

#include "gtest/gtest.h"

#include <opencv2/core.hpp>
#include "gk_types.h"
#include "gk_operators.h"

TEST(basic_check, test_equal) {

    cv::Mat p1 = cv::Mat::ones(3,3,gekon::KERNEL_TYPE);
    cv::Mat p2 = cv::Mat::ones(3,3,gekon::KERNEL_TYPE);
    p1 = 2*p1;
    p2 = 3*p2;
    auto kids = gekon::c_simle(p1, p2);
    bool accumulator = true;
    for (int j = 0; j < 3; ++j) {
        for (int k = 0; k < 3; ++k) {
            bool k1 = kids[0].at<gekon::ker_num_t>(j,k) == p1.at<gekon::ker_num_t>(j,k) ||
                      kids[0].at<gekon::ker_num_t>(j,k) == p2.at<gekon::ker_num_t>(j,k);
            bool k2 = kids[1].at<gekon::ker_num_t>(j,k) == p1.at<gekon::ker_num_t>(j,k) ||
                      kids[1].at<gekon::ker_num_t>(j,k) == p2.at<gekon::ker_num_t>(j,k);
            accumulator = accumulator & k1 & k2;
        }
    }

    EXPECT_EQ(accumulator,true);
}

