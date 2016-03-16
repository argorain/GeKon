//
// Created by sehny on 2/24/16.
//

#ifndef GEKON_GA_FUNCTIONS_H
#define GEKON_GA_FUNCTIONS_H

#include <vector>
#include <functional>
#include <opencv2/core/mat.hpp>
#include "gk_utils.h"
#include "gk_types.h"

namespace gekon
{

    // Note about returning vector as value:
    // http://thbecker.net/articles/rvalue_references/section_06.html
    // http://stackoverflow.com/questions/6211575/proper-way-move-semantics-to-return-a-stdvector-from-function-calling-in-c

    // initialize new population
    population_t first_generation(const size_t size, int ksize);

    bool cmp_candidates(const std::pair<double, candidate_t> a,
                        const std::pair<double, candidate_t> b);

    typedef std::function<double(tr_sample_t, candidate_t)> fitness_fcn_t;

    // apply fitness function to the whole generation
    /*std::vector<double> fitness(fitness_fcn_t fit_fcn,
                            std::vector<tr_sample_t> samples,
                            const population_t generation);*/
    void fitness(fitness_fcn_t fit_fcn,
                 tr_sample_t sample,
                 population_t generation);

    // select candidates for new generation
    // implements also elitism
    typedef std::function<population_t(population_t)> selection_fcn_t;

    population_t s_roulette(const population_t prev_population);

    // Naive fitness function implementing mean square error calculation
    double fitness_mse(const tr_sample_t sample, const candidate_t candidate);

}

#endif //GEKON_GA_FUNCTIONS_H
