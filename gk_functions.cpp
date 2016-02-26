//
// Created by sehny on 2/25/16.
//

#include "gk_functions.h"
#include "gk_utils.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

using namespace gekon;
using namespace cv;

// I thought it'd be needed. I was wrong.
template <typename T>
T sum_vec(std::vector<T> vec)
{
    T ret = 0;
    std::for_each(vec.begin(), vec.end(), [&](auto it){ret += it;});
    return ret;
}

std::vector<double> fitness(std::function<double(tr_sample_t, candidate_t)> fit_fcn,
                            std::vector<tr_sample_t> samples,
                            const population_t generation)
{
    // Fitness matrix; 1st dimension - samples; 2nd dim - candidates applied to the given
    // sample
    std::vector<std::vector<double>> fit_matrix;
    fit_matrix.resize(samples.size());

    for (unsigned int i = 0; i < samples.size(); ++i) {
        fit_matrix[i].resize(generation.size());
        std::transform(generation.begin(), generation.end(), fit_matrix[i].begin(),
                       [&](auto iter){return fit_fcn(samples[i], iter);});

    }

    // Sum every column and calculate mean of fitness values for each candidate
    // Right now it's just dumb arithmetic mean
    std::vector<double> fit_result;
    fit_result.resize(generation.size());
    for (unsigned int i = 0; i < generation.size(); ++i) {
        double fit_sum = 0;
        for (unsigned int j = 0; j < samples.size(); ++j) {
            fit_sum += fit_matrix[j][i];
        }
        fit_result[i] = fit_sum/double(samples.size());
    }
    return fit_result;
}


double fitness_mse(const tr_sample_t sample, const candidate_t candidate)
{
    //conv2 with original
    //diff = A-B
    //sum(sum(diff))

    Mat conv_result;
    filter2D(sample.original, conv_result, -1, candidate, Point(-1,-1), 0, BORDER_CONSTANT);
    conv_result -= sample.modified;

    double sum_square = 0;
    MatConstIterator_<unsigned char> it = conv_result.begin<unsigned char>(), it_end = conv_result.end<unsigned char>();
    for(; it != it_end; ++it)
    {
        sum_square += double(*it)*double(*it);
    }
    double mse=sqrt(sum_square/double(conv_result.rows*conv_result.cols));
    return mse;
}




