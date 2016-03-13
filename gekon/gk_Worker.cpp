//
// Created by sehny on 3/12/16.
//

#include "gk_Worker.h"

#include <vector>

using namespace std;

namespace gekon {

    Worker::Worker(size_t gensize, int ksize) {
        //TODO: select =
        crossover = c_blx_a;
        mutate = m_swap;
        population = first_generation(gensize, ksize);
        fit_single = fitness_mse;
    }

    Worker::~Worker() {

    }

    double Worker::run() {
        static bool first_run = true;
        vector<double> fit_values;

        if (first_run) {
            fit_values = fitness(fit_single, samples, population);
            // min element fcn returns iterator, so I have to
            // dereference it to get double
            return *min_element(fit_values.begin(), fit_values.end());
        }

        population_t new_generation = select(population);
        population_t new_kernels;

        // take pair of kernels and produce new pair
        for (unsigned int j = 0; j < new_generation.size(); j += 2) {
            population_t new_pair = crossover(new_generation[j], new_generation[j + 1]);
            new_kernels.insert(new_kernels.end(), new_pair.begin(), new_pair.end());
        }
        for_each(new_kernels.begin(),
                new_kernels.end(),
                [](auto &iter){
                    iter = mutate(iter);
                });

        new_generation.insert(new_generation.end(), new_kernels.begin(), new_kernels.end());

        return 0;
    }
}
