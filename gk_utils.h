//
// Created by sehny on 2/24/16.
//

#ifndef GEKON_UTILS_H
#define GEKON_UTILS_H

#include <tuple>
#include <iostream>
#include <opencv2/core.hpp>

namespace gekon
{
    static const std::string img_type = ".png";
    static const std::string modified_img = ".mod";

    // training sample
    // cv::Mat use references instead of copying so we don't have to
    // worry about memory management (right ?)
    typedef std::tuple<cv::Mat, cv::Mat> tr_sample_t;

    // load an image and its modified version; return them as a pair
    tr_sample_t load_img_pair(const std::string name);

    // open directory, find all suitable files based on filename pattern and load them
    std::vector<tr_sample_t> load_samples (const std::string dir_name);
}

#endif //GEKON_UTILS_H
