//
// Created by sehny on 2/24/16.
//

#ifndef GEKON_GA_OPERATORS_H
#define GEKON_GA_OPERATORS_H

#include <functional>
#include "gk_functions.h"

namespace gekon
{

    //float random(float min, float max);
    ker_num_t random(ker_num_t min, ker_num_t max);

    //crossover and mutation
    typedef std::function<std::vector<candidate_t>(candidate_t, candidate_t)> crossover_fcn_t;
    typedef std::function<void(candidate_t&, unsigned int, unsigned int)> mutation_fcn_t;

    // Parameters
    const float c_blx_a_alpha = 0.5; // Empiric paramter to widen range	
    const float m_swap_prob = 0.1; // Probability of swap mutation
    const float m_dynamic_B = 5;

    // crossovers
    // BLX-a and BLX-a-b source http://www.tomaszgwiazda.com/blendX.htm
    std::vector<candidate_t> c_blx_a(candidate_t X, candidate_t Y);

    std::vector<candidate_t> c_simle(candidate_t X, candidate_t Y);

    std::vector<candidate_t> c_convex(candidate_t X, candidate_t Y);

    // mutations
    void m_swap(candidate_t &X, unsigned int t, unsigned int T);
    void m_dynamic(candidate_t &X, unsigned int t, unsigned int T);
}

#endif //GEKON_GA_OPERATORS_H
