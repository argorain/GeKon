//
// Created by sehny on 3/12/16.
//

#include "gk_Worker.h"

#include <vector>

using namespace std;

namespace gekon {

    Worker::Worker() {
        select = s_roulette;
        crossover = c_blx_a;
        mutate = m_swap;
        //population = first_generation(gensize, ksize);
        fit_single = fitness_ssim;
        kernel_size = 3;
        generation_size = 100;

        static_assert((unsigned int) NUM_THREADS > 0, "Number of threads must be numeric value greater than zero.");
        threads = NUM_THREADS;
        first_run = true;
        max_generation_count = 5000;
        generation_counter = 1;

    }

    Worker::~Worker() {

    }

    double Worker::run() {
        cout << "Run!" << endl;
        if (first_run) {
            cout << "Running with " << threads << " threads." << endl;
            population = first_generation(generation_size, kernel_size);
            fitness(fit_single, samples, population, threads);
            // min element fcn returns iterator, so I have to
            // dereference it to get double
            //return *min_element(fit_values.begin(), fit_values.end());
            first_run = false;
            cout << "Init first gen!" << endl;
            return population[0].first;
        }

        cout << "Select!" << endl;
        pair<double, candidate_t> elite = population[0];
        population_t new_generation = select(population);
        cout << "Population fitness: " << "[" << elite.first << "], ";
        for_each(population.begin(), population.end(), [](auto &iter) {
            cout << iter.first << ", ";
        });
        cout << endl;
        vector <candidate_t> new_kernels;
        population_t new_kernels_with_fit;

        cout << "Crossover" << endl;
        // take pair of kernels and produce new pair
        cout << "New gen size: " << new_generation.size() << endl;
        for (unsigned int j = 0; j < new_generation.size(); j += 2) {
            auto new_pair = crossover(new_generation[j].second, new_generation[j + 1].second);
            //cout << "new pair" << new_pair[0] << endl;
            new_kernels.insert(new_kernels.end(), new_pair.begin(), new_pair.end());
        }
        cout << "New kernels size: " << new_kernels.size() << endl;

        // You should mutate everyone! There is some probability of mutation, but it should by applied for eveyone
        // todo: do not calculate fitness for those who haven't been changed by mutation => remember
        // who was mutated
        for_each(new_kernels.begin(),
                 new_kernels.end(),
                 [=](auto &iter) {
                     mutate(iter, generation_counter, max_generation_count);
                 });

        new_kernels_with_fit.resize(new_kernels.size());
        transform(new_kernels.begin(),
                  new_kernels.end(),
                  new_kernels_with_fit.begin(),
                  [](auto iter) { return make_pair(0, iter); });

        fitness(fit_single, samples, new_kernels_with_fit, threads);

        new_generation.insert(new_generation.end(), new_kernels_with_fit.begin(), new_kernels_with_fit.end());
        new_generation.push_back(elite);
        std::sort(new_generation.begin(), new_generation.end(), cmp_candidates);
        population = new_generation;

        cout << "Gen size: " << new_generation.size() << endl;

        ++generation_counter;

        return population[0].first;
    }
}
