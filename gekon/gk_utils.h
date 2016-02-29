//
// Created by sehny on 2/24/16.
//

#ifndef GEKON_UTILS_H
#define GEKON_UTILS_H

#include <tuple>
#include <iostream>
#include <opencv2/core.hpp>
#include "gk_types.h"

namespace gekon
{
    static const std::string img_type = ".png";
    static const std::string modified_img = ".mod";


    // load an image and its modified version; return them as a pair
    tr_sample_t load_img_pair(const std::string name);

    // open directory, find all suitable files based on filename pattern and load them
    std::vector<tr_sample_t> load_samples (const std::string dir_name);

	tr_sample_t* load_img_pair(const std::string original, const std::string modified);
}

#endif //GEKON_UTILS_H
