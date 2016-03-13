//
// Created by sehny on 3/12/16.
//

#ifndef GEKON_GK_WORKER_H
#define GEKON_GK_WORKER_H

#include "gk_types.h"
#include "gk_functions.h"
#include "gk_operators.h"
#include "gk_utils.h"

namespace gekon {

    class Worker {
    private:
        int kernel_size;
        size_t generation_size;
        population_t population;

        std::vector<tr_sample_t> samples;

        selection_fcn_t select;
        crossover_fcn_t crossover;
        mutation_fcn_t mutate;

        fitness_fcn_t fit_single;

    public:
        Worker(size_t gensize, int ksize);
        virtual ~Worker();
        void setSelectionFcn(selection_fcn_t fcn) {select = fcn;};
        void setCrossoverFcn(crossover_fcn_t fcn) {crossover = fcn;};
        void setFcn(mutation_fcn_t fcn) {mutate = fcn;};
        double run();
    };
}

#endif //GEKON_GK_WORKER_H
