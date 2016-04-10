//
// Created by sehny on 2/25/16.
//

#include "gk_functions.h"
#include "gk_utils.h"
#include "gk_types.h"
#include "gk_operators.h"

#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include <boost/container/map.hpp>
#include <thread>

using cv::Mat; using cv::Mat_;

namespace gekon {



	population_t normalize(population_t values);

    // I thought it'd be needed. I was wrong.
    template<typename T>
    T sum_vec(std::vector<T> vec) {
        T ret = 0;
        std::for_each(vec.begin(), vec.end(), [&](auto it) { ret += it; });
        return ret;
    }

    population_t first_generation(const size_t size, int ksize) {
        population_t new_population;
        new_population.resize(size);
        for (unsigned int j = 0; j < size; ++j) {
            auto new_kernel = Mat(ksize,ksize,KERNEL_TYPE);
            // generate random matrix with lower bound 0, upper bound 255 and uniform random distribution
            cv::randu(new_kernel, cv::Scalar::all(-20), cv::Scalar::all(20));
            new_population[j] = std::make_pair(0, new_kernel);
        }
        return new_population;
    }

    /*std::vector<double> fitness(std::function<double(tr_sample_t, candidate_t)> fit_fcn,
                                std::vector<tr_sample_t> samples,
                                const population_t generation) {
        // Fitness matrix; 1st dimension - samples; 2nd dim - candidates applied to the given
        // sample
        std::vector<std::vector<double>> fit_matrix;
        fit_matrix.resize(samples.size());

        for (unsigned int i = 0; i < samples.size(); ++i) {
            fit_matrix[i].resize(generation.size());
            std::transform(generation.begin(), generation.end(), fit_matrix[i].begin(),
                           [&](auto iter) { return fit_fcn(samples[i], iter); });

        }

        // Sum every column and calculate mean of fitness values for each candidate
        // Right now it's just dumb arithmetic mean
        std::vector<double> fit_result;
        fit_result.resize(generation.size());
        for (unsigned int i = 0; i < generation.size(); ++i) {
            double fit_sum = 0;
            for (unsigned int j = 0; j < samples.size(); ++j) {
                fit_sum += fit_matrix[j][i];
            }
            fit_result[i] = fit_sum / double(samples.size());
        }
        return fit_result;
    }*/



    double fitness_mse(const tr_sample_t sample, const candidate_t candidate) {
        //conv2 with original
        //diff = A-B
        //sum(sum(diff))

        Mat_<ker_num_t> conv_result;
        //Mat conv_result;
        filter2D(sample.original, conv_result, -1, candidate, cv::Point(-1, -1), 0, cv::BORDER_CONSTANT);

        //std::cout << candidate << std::endl;
        /*
        imshow("Image", sample.original);
        cv::waitKey(0);
        imshow("Image", sample.modified);
        cv::waitKey(0);
        */

        //imshow("Image", conv_result);
        //cv::waitKey(0);

        double sum_square = 0;
        cv::MatConstIterator_<ker_num_t> it = conv_result.begin(),
                it_end = conv_result.end(),
                it2 = sample.modified.begin<ker_num_t>(),
                it2_end = sample.modified.end<ker_num_t>();
        for (; it != it_end && it2 != it2_end; ++it, ++it2) {
            sum_square += pow(double(*it)-double(*it2), 2);
        }

        /*
        conv_result = conv_result - sample.modified;
        cv::pow(conv_result, 2, conv_result);
        double sum_square = cv::sum(conv_result)[0]; //zero means first channel
        // http://stackoverflow.com/questions/11071509/opencv-convert-scalar-to-float-or-double-type
         */

        double mse = sqrt(double(conv_result.rows * conv_result.cols)/(sum_square+0.0001));
        //std::cout << "MSE:" << mse;
        //return mse;
        return exp(5*mse);
    }


	population_t normalize(population_t values) {
		population_t norm;
        double sum = 0;

        for(auto const& val: values) {
            sum += val.first;
        }

		for(auto const& val: values) {
			norm.push_back(std::make_pair(val.first/sum, val.second ));
		}

		return norm;
	}

    population_t s_roulette(const population_t prev_population) {

		population_t normPop = normalize(prev_population);
        population_t selPop;

        double sfitt = 0;
        int left = 0;
        if(normPop.size()/2 % 2 == 0)
            left = normPop.size()/2;
        else
            left = normPop.size()/2+1;

        for(int i=0; i<(int)normPop.size(); i++) {
            sfitt += normPop[i].first;
            normPop[i].first = sfitt;
        }

        std::cout << "Selected positions: ";
        for(int i=0; i<left; i++) {
            double random = gekon::random(0, 1);
            /*for(auto const& val: normPop) {
                if(val.first > random) {
                    selPop.push_back(std::make_pair(val.first, val.second));
                    break;
                }
            }*/
            for (unsigned int j = 0; j < normPop.size(); ++j) {
                if (normPop[j].first > random) {
                    selPop.push_back(std::make_pair(normPop[j].first, normPop[j].second));
                    std::cout << j << ", ";
                    break;
                }
            }
        }
        std::cout << std::endl;

		return selPop;
	}

    population_t s_tournament(const population_t prev_population) {
        unsigned int new_gen_size;
        unsigned int prev_gen_size = (unsigned int)prev_population.size();

        if(prev_gen_size/2 % 2 == 0)
            new_gen_size = prev_gen_size/2;
        else
            new_gen_size = prev_gen_size/2+1;

        population_t new_gen;
        new_gen.resize(new_gen_size);

        for (unsigned int j = 0; j < new_gen_size; ++j) {
            unsigned int fst = (unsigned int)floor(random(0, prev_gen_size)),
                snd = (unsigned int)floor(random(0, prev_gen_size));
            new_gen[j] = prev_population[(fst < snd) ? fst : snd];
        }
        return new_gen;
    }

    population_t s_rank_selection(const population_t prev_population) {
        population_t mod_pop; // modified population - replace fitness with reversed index
        size_t pop_size = prev_population.size();
        mod_pop.resize(prev_population.size());
        for (size_t j = 0; j < pop_size; ++j) {
            // now perform the replacement
            mod_pop[j].first = double(pop_size-j);
            mod_pop[j].second = prev_population[j].second;
        }
        population_t ret_pop = s_roulette(mod_pop);
        // use already implemented roulette selection
        for (size_t j = 0; j < ret_pop.size(); ++j) {
            // pick fitness values back from previous population
            size_t prev_index = pop_size - size_t(ret_pop[j].first);
            ret_pop[j].first = prev_population[prev_index].first;
        }
        return ret_pop;
    }

    bool cmp_candidates(const std::pair<double, candidate_t> a, const std::pair<double, candidate_t> b) {
        double fit_a, fit_b;
        fit_a = a.first;
        fit_b = b.first;
        return fit_a > fit_b; //reverse the order because bigger fitness means better cadidate
    }

    static inline double fitness_vec(std::vector<tr_sample_t> samples,
                                     candidate_t candidate,
                                     fitness_fcn_t fit_fcn)
    {
        double ret_value = 0;
        std::for_each(samples.begin(), samples.end(), [&](auto &iter){
           ret_value += fit_fcn(iter, candidate);
        });
        ret_value /= double(samples.size());
        return ret_value;
    }

    void fitness(fitness_fcn_t fit_fcn,
                 //tr_sample_t sample,
                 std::vector<tr_sample_t> samples,
                 population_t &generation,
                 const unsigned int threads_num)
    {
        size_t vec_size = generation.size();
        unsigned int num_of_threads = threads_num;
        std::vector<size_t> boundaries;

        unsigned int step = (unsigned int)vec_size/num_of_threads;
        for (unsigned int i = 0; i < num_of_threads; ++i) {
            //fixme: last thread has biggest portion
            boundaries.push_back(i*step);
        }
        boundaries.push_back(vec_size);
        std::thread *tt = new std::thread[num_of_threads];

        for (unsigned int i = 0; i < num_of_threads; ++i) {
            tt[i] = std::thread([&](size_t low, size_t high){
                for (size_t j = low; j < high; ++j) {
                    //generation[j].first = fit_fcn(sample, generation[j].second);
                    generation[j].first = fitness_vec(samples, generation[j].second, fit_fcn);
                }
            }, boundaries[i], boundaries[i+1]);
        }
        //fixme: main thread is idle
        for (unsigned int i = 0; i < num_of_threads; ++i)
            tt[i].join();

        delete [] tt;


        /*
        for (size_t j = 0; j < generation.size(); ++j) {
            // std::cout << "Fitness calc #" << j;
            generation[j].first = fit_fcn(sample, generation[j].second);
            //std::cout << " = " << generation[j].first << std::endl;
            //std::cout << "Konvolution" << std::endl << generation[j].second << std::endl;
        }*/
        //std::sort(generation.begin(), generation.end(), cmp_candidates);
    }
}
