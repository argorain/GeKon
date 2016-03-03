#include "gtest/gtest.h"

#include <cmath>
#include <opencv2/core.hpp>
#include "gk_types.h"
#include "gk_functions.h"
#include "gk_utils.h"

// Try load two same pictures. Pixel-by-pixel subtraction should ends with 0
TEST(load_img_pair, test_equal) {

	std::string imagename = "lena";
	std::string path = "test_samples/";

	gekon::tr_sample_t sample = gekon::load_img_pair(path, imagename);

	int diff = 0;

	int rows = sample.original.rows;
	int cols = sample.original.cols;

	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			diff += sample.original.at<cv::Vec3b>(i,j)[0] - sample.modified.at<cv::Vec3b>(i,j)[0];
		}
	}

	EXPECT_EQ(diff, 0);
}

// Try to load something from wrong folder.
TEST(load_img_pair, test_neq) {
	int result = 0;

	std::string imagename = "igor";
	std::string path = "test_corpses";

	try {
		gekon::tr_sample_t sample = gekon::load_img_pair(path, imagename);
	} catch(std::exception& e) {
		result = -1;
	}

	EXPECT_NE(result, 0);
}

// Try load image pairs from test_samples. There should be 2 valid pairs.
TEST(load_samples, test_eq) {
	std::string dir = "test_samples/";
	
	std::vector<gekon::tr_sample_t> pairs = gekon::load_samples(dir);

	EXPECT_EQ(pairs.size(), 2);
}
