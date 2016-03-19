//
// Created by sehny on 3/12/16.
//

#include "gk_Worker.h"

#include <vector>

using namespace std;

namespace gekon {

    Worker::Worker(size_t gensize, int ksize) {
        select = s_roulette;
        crossover = c_blx_a;
        mutate = m_swap;
        population = first_generation(gensize, ksize);
        fit_single = fitness_mse;

       /* 
        for (auto &iter: population) {
            cout << iter.first << ' ' << iter.second << endl;
        }*/
    }

    Worker::~Worker() {

    }

    double Worker::run() {
        static bool first_run = true;
        vector<double> fit_values;

        cout << "Run!" << endl;
        if (first_run) {
            fitness(fit_single, sample, population);
            // min element fcn returns iterator, so I have to
            // dereference it to get double
            //return *min_element(fit_values.begin(), fit_values.end());
            first_run = false;
            cout << "Init first gen!" << endl;
            return population[0].first;
        }

        cout << "Evolve!" << endl;
        population_t new_generation = select(population);
        vector<candidate_t> new_kernels;
        population_t new_kernels_with_fit;


        for (auto &iter: population) {
            cout << iter.first << ' ' << iter.second << endl;
        }

        // take pair of kernels and produce new pair
        for (unsigned int j = 0; j < new_generation.size(); j += 2) {
            auto new_pair = crossover(new_generation[j].second, new_generation[j + 1].second);
            new_kernels.insert(new_kernels.end(), new_pair.begin(), new_pair.end());
        }
        // TODO: do not mutate everyone
        for_each(new_kernels.begin(),
                new_kernels.end(),
                [=](auto &iter){
                    mutate(iter);
                });
        transform(new_kernels.begin(),
                  new_kernels.end(),
                  new_kernels_with_fit.begin(),
                  [](auto iter){return make_pair(0, iter);});

        fitness(fit_single, sample, new_kernels_with_fit);
        new_generation.insert(new_generation.end(), new_kernels_with_fit.begin(), new_kernels_with_fit.end());
        std::sort(new_generation.begin(), new_generation.end(), cmp_candidates);

        return new_generation[0].first;
    }
}
