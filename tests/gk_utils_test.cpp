#include "gtest/gtest.h"

#include <cmath>
#include <opencv2/core.hpp>
#include "gk_types.h"
#include "gk_functions.h"
#include "gk_utils.h"
/*
TEST(basic_check, test_eq) {
    EXPECT_EQ(1,0);
}

TEST(basic_check, test_neq) {
    EXPECT_NE(1,0);
}
*/
TEST(load_img_pair, test_equal) {
	std::string imagename = "lena_gray";

	gekon::tr_sample_t sample = gekon::load_img_pair(imagename);

	int diff = 0;

	int rows = sample.original.rows;
	int cols = sample.original.cols;

	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			diff += sample.original.at<cv::Vec3b>(i,j)[0];
			//diff += sample->original.at(i,j)[1] - sample->modified.at(i,j)[1];
			//diff += sample->original.at(i,j)[2] - sample->modified.at(i,j)[2];
		}
	}

	EXPECT_EQ(diff, 0);
}
