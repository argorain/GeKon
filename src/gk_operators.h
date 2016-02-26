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
    typedef std::function<candidate_t(candidate_t, candidate_t)> crossover_fcn_t;
    typedef std::function<void(candidate_t&)> mutation_fcn_t;
}

#endif //GEKON_GA_OPERATORS_H
