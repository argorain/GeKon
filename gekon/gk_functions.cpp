//
// Created by sehny on 2/25/16.
//

#include "gk_functions.h"
#include "gk_utils.h"
#include "gk_types.h"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include <boost/container/map.hpp>

using cv::Mat; using cv::Mat_;

namespace gekon {


	std::vector<double> normalize(std::vector<double> values);

    // I thought it'd be needed. I was wrong.
    template<typename T>
    T sum_vec(std::vector<T> vec) {
        T ret = 0;
        std::for_each(vec.begin(), vec.end(), [&](auto it) { ret += it; });
        return ret;
    }

    population_t first_generation(const size_t size) {
        std::vector<candidate_t> new_population;
        new_population.resize(size);
        for (unsigned int j = 0; j < size; ++j) {
            //fixme: hard-coded matrix size
            new_population[j] = Mat(3,3,CV_8U);
            // generate random matrix with lower bound 0, upper bound 255 and uniform random distribution
            cv::randu(new_population[j], cv::Scalar::all(0), cv::Scalar::all(255));
        }
        return new_population;
    }

    std::vector<double> fitness(std::function<double(tr_sample_t, candidate_t)> fit_fcn,
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
    }


    double fitness_mse(const tr_sample_t sample, const candidate_t candidate) {
        //conv2 with original
        //diff = A-B
        //sum(sum(diff))

        Mat_<unsigned char> conv_result;
        filter2D(sample.original, conv_result, -1, candidate, cv::Point(-1, -1), 0, cv::BORDER_CONSTANT);

        double sum_square = 0;
        cv::MatConstIterator_<unsigned char> it = conv_result.begin(),
                it_end = conv_result.end(),
                it2 = sample.modified.begin<unsigned char>(),
                it2_end = sample.modified.end<unsigned char>();
        for (; it != it_end && it2 != it2_end; ++it, ++it2) {
            sum_square += pow(double(*it)-double(*it2), 2);
        }
        double mse = sqrt(sum_square / double(conv_result.rows * conv_result.cols));
        return mse;
    }


	std::vector<double> normalize(std::vector<double> values) {
		std::vector<double> norm;
		double max = *std::max_element(values.begin(), values.end());	

		for(auto const& val: values) {
			norm.push_back(val/max);
		}

		return norm;
	}

    population_t s_roulette(const population_t prev_population, std::vector<double> fitness) {
		population_t newPop;

		std::vector<double> normFitt = normalize(fitness);	
//DEPRECATED
		/*	
		std::map<double, candidate_t>  generation;

		for(int i=0; i<(int)normFitt.size(); ++i) {
			generation[normFitt.at(i)] = prev_population.at(i);
		}

		for( std::map<double, candidate_t>::iterator ii=generation.begin(); ii!=generation.end(); ++ii)
		{
			std::cout << (*ii).first << ": " << (*ii).second << std::endl;
	 	}		

*/

		return newPop;
	}
}
