//
// Created by sehny on 2/27/16.
//

#ifndef GEKON_GK_TYPES_H
#define GEKON_GK_TYPES_H

#include <opencv2/core.hpp>
#include <vector>

namespace gekon
{
    // types used further in genetic algorithms
    typedef cv::Mat candidate_t;
    typedef std::vector<std::pair<double, candidate_t>> population_t;

    // training sample
    // cv::Mat use references instead of copying so we don't have to
    // worry about memory management (right ?)
    typedef struct{
        cv::Mat original;
        cv::Mat modified;
    } tr_sample_t;

    typedef float ker_num_t; //one number in convolution kernel
    static const auto KERNEL_TYPE = CV_32F;

}

#endif //GEKON_GK_TYPES_H
