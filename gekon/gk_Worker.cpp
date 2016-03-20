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
        //vector<double> fit_values;

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

        cout << "Select!" << endl;
        auto elite = population[0];
        population_t new_generation = select(population);
        vector<candidate_t> new_kernels;
        population_t new_kernels_with_fit;


        for (auto &iter: population) {
            cout << iter.first << ' ' << iter.second << endl;
        }

        cout << "Crossover" << endl;
        // take pair of kernels and produce new pair
        for (unsigned int j = 0; j < new_generation.size(); j += 2) {
            auto new_pair = crossover(new_generation[j].second, new_generation[j + 1].second);
            cout << "new pair" << new_pair[0];
            new_kernels.insert(new_kernels.end(), new_pair.begin(), new_pair.end());
        }

        //cout << new_kernels[0] << endl;
        //cout << "new kernels" << endl;
        //for (size_t i=0; i < 5; ++i) {
        //    cout << new_kernels[i] << endl;
        //}

        // TODO: do not mutate everyone
        // You should mutate everyone! There is some probability of mutation, but it should by applied for eveyone
        for_each(new_kernels.begin(),
                new_kernels.end(),
                [=](auto &iter){
                    mutate(iter);
                });
        
        new_kernels_with_fit.resize(new_kernels.size());
        transform(new_kernels.begin(),
                  new_kernels.end(),
                  new_kernels_with_fit.begin(),
                  [](auto iter){return make_pair(0, iter);});

        fitness(fit_single, sample, new_kernels_with_fit);
        
        cout << "new kernels" << endl;
        for (size_t i=0; i < (size_t)new_kernels.size(); ++i) {
            cout << new_kernels[i] << endl;
        }
        
        new_generation.insert(new_generation.end(), new_kernels_with_fit.begin(), new_kernels_with_fit.end());
        new_generation.push_back(elite);
        std::sort(new_generation.begin(), new_generation.end(), cmp_candidates);

        
        cout << "Population:" << endl;
        for (size_t i=0; i < (size_t)new_generation.size(); ++i) {
            cout << new_generation[i].first << endl;
            cout << new_generation[i].second << endl;
        }

        return new_generation[0].first;
    }
}
