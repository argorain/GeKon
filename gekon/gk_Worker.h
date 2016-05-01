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

        std::vector <tr_sample_t> samples;
        tr_sample_t sample;

        selection_fcn_t select;
        crossover_fcn_t crossover;
        mutation_fcn_t mutate;

        fitness_fcn_t fit_single;

        unsigned int threads;
        bool first_run;

        unsigned int generation_counter, max_generation_count;

    public:
        Worker();

        virtual ~Worker();

        void setKernelSize(const int ksize) { kernel_size = ksize; };

        void setGenSize(const size_t gensize) { generation_size = gensize; };

        void setTrSample(tr_sample_t asample) {
            samples.push_back(asample);
        };

        void setTrSamples(auto asamples) {
            samples = asamples;
        }

        void setSelectionFcn(selection_fcn_t fcn) { select = fcn; };

        void setCrossoverFcn(crossover_fcn_t fcn) { crossover = fcn; };

        void setMutationFcn(mutation_fcn_t fcn) { mutate = fcn; };

        void overrideNumOfThreads(const unsigned int n) { threads = n; };

        void setMaxNumberOfGenerations(const unsigned int n) { max_generation_count = n; };

        void setFitnessFcn(fitness_fcn_t fcn) { fit_single = fcn; };

        double run();

        cv::Mat retBestSolution() { return population[0].second; };

        cv::Mat retWorstSolution() { return population[population.size() - 1].second; };
    };
}

#endif //GEKON_GK_WORKER_H
