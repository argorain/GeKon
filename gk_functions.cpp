//
// Created by sehny on 2/25/16.
//

#include "gk_functions.h"
#include <opencv2/core.hpp>

using namespace gekon;

std::vector<double> fitness(std::function<double(tr_sample_t, candidate_t)> fit_fcn,
                            tr_sample_t sample,
                            const population_t generation)
{
    std::vector<double> fit_result;
    fit_result.resize(generation.size());
    std::transform(generation.begin(), generation.end(), fit_result.begin(),
                    [&](auto iter){return fit_fcn(sample, iter);});
    return fit_result;
}


double fitness_mse(tr_sample_t sample, candidate_t candidate)
{
    double diff;
    //conv2 with original
    //diff = A-B
    //sum(sum(diff))
    return diff;
}




