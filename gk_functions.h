//
// Created by sehny on 2/24/16.
//

#ifndef GEKON_GA_FUNCTIONS_H
#define GEKON_GA_FUNCTIONS_H

#include <vector>
#include <functional>
#include <opencv2/core/mat.hpp>
#include "gk_utils.h"


namespace gekon
{
    // types used further in genetic algorithms
    typedef cv::Mat candidate_t;
    typedef std::vector<candidate_t> population_t;

    // Note about returning vector as value:
    // http://thbecker.net/articles/rvalue_references/section_06.html
    // http://stackoverflow.com/questions/6211575/proper-way-move-semantics-to-return-a-stdvector-from-function-calling-in-c

    // initialize new population
    population_t first_generation(const size_t size);

    // apply fitness function to the whole generation
    std::vector<double> fitness(std::function<double(tr_sample_t, candidate_t)> fit_fcn, 
            const population_t generation);

    // select candidates for new generation
    // implements also elitism
    population_t selection(const population_t prev_population);

    // Naive fitness function implementing mean square error calculation
    double fitness_mse(tr_sample_t sample, candidate_t candidate);

}

#endif //GEKON_GA_FUNCTIONS_H
