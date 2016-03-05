//
// Created by sehny on 2/24/16.
//

#ifndef GEKON_GA_OPERATORS_H
#define GEKON_GA_OPERATORS_H

#include <functional>
#include "gk_functions.h"

namespace gekon
{
    //crossover and mutation
    typedef std::function<std::vector<candidate_t>(candidate_t, candidate_t)> crossover_fcn_t;
    typedef std::function<void(candidate_t&)> mutation_fcn_t;

	std::vector<candidate_t> blx_a(candidate_t X, candidate_t Y);
}

#endif //GEKON_GA_OPERATORS_H
