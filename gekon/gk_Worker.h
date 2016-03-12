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

    class gk_Worker {
    private:
        population_t population;
        selection_fcn_t select;
        crossover_fcn_t crossover;
        mutation_fcn_t mutate;

    public:
        gk_Worker();
        virtual ~gk_Worker();
        void setSelectionFcn(selection_fcn_t fcn) {select = fcn;};
        void setCrossoverFcn(crossover_fcn_t fcn) {crossover = fcn;};
        void setFcn(mutation_fcn_t fcn) {mutate = fcn;};
        //TODO: some run or iter function that returns best result in given generation
    };
}

#endif //GEKON_GK_WORKER_H
