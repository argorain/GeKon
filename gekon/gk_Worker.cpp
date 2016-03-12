//
// Created by sehny on 3/12/16.
//

#include "gk_Worker.h"

#include <vector>

using namespace std;

namespace gekon {

    Worker::Worker(size_t s=1000) {
        //TODO: select =
        crossover = c_blx_a;
        mutate = m_swap;
        population = first_generation(s);
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

        return 0;
    }
}
