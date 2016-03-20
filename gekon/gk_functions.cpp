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

        std::cout << candidate << std::endl;
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
        std::cout << "MSE:" << mse;
        return mse;
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

        for(int i=0; i<left; i++) {
            double random = gekon::random(0, 1);
            for(auto const& val: normPop) {
                if(val.first > random) {
                    selPop.push_back(std::make_pair(val.first, val.second));
                    break;
                }
            }
        }

		return selPop;
	}

    bool cmp_candidates(const std::pair<double, candidate_t> a, const std::pair<double, candidate_t> b) {
        double fit_a, fit_b;
        fit_a = a.first;
        fit_b = b.first;
        return fit_a < fit_b;
    }

    void fitness(fitness_fcn_t fit_fcn, tr_sample_t sample, population_t &generation) {
        for (size_t j = 0; j < generation.size(); ++j) {
            std::cout << "Fitness calc #" << j;
            generation[j].first = fit_fcn(sample, generation[j].second);
            std::cout << " = " << generation[j].first << std::endl;
            //std::cout << "Konvolution" << std::endl << generation[j].second << std::endl;
        }
        //std::sort(generation.begin(), generation.end(), cmp_candidates);
    }
}
