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
    T sum_vec(std::vector <T> vec) {
        T ret = 0;
        std::for_each(vec.begin(), vec.end(), [&](auto it) { ret += it; });
        return ret;
    }

    population_t first_generation(const size_t size, int ksize) {
        population_t new_population;
        new_population.resize(size);
        for (unsigned int j = 0; j < size; ++j) {
            auto new_kernel = Mat(ksize, ksize, KERNEL_TYPE);
            // generate random matrix with lower bound 0, upper bound 255 and uniform random distribution
            cv::randu(new_kernel, cv::Scalar::all(-20), cv::Scalar::all(20));
            new_population[j] = std::make_pair(0, new_kernel);
        }
        return new_population;
    }

    double fitness_mse(const tr_sample_t sample, const candidate_t candidate) {

        Mat_ <ker_num_t> conv_result;
        filter2D(sample.original, conv_result, -1, candidate, cv::Point(-1, -1), 0, cv::BORDER_CONSTANT);

        double sum_square = 0;
        cv::MatConstIterator_ <ker_num_t> it = conv_result.begin(),
                it_end = conv_result.end(),
                it2 = sample.modified.begin<ker_num_t>(),
                it2_end = sample.modified.end<ker_num_t>();
        for (; it != it_end && it2 != it2_end; ++it, ++it2) {
            sum_square += pow(double(*it) - double(*it2), 2);
        }

        double mse = sqrt(double(conv_result.rows * conv_result.cols) / (sum_square + 0.0001));
        return exp(5 * mse);
    }


    population_t normalize(population_t values) {
        population_t norm;
        double sum = 0;

        for (auto const &val: values) {
            sum += val.first;
        }

        for (auto const &val: values) {
            norm.push_back(std::make_pair(val.first / sum, val.second));
        }

        return norm;
    }

    population_t s_roulette(const population_t prev_population) {

        population_t normPop = normalize(prev_population);
        population_t selPop;

        double sfitt = 0;
        int left = 0;
        if (normPop.size() / 2 % 2 == 0)
            left = normPop.size() / 2;
        else
            left = normPop.size() / 2 + 1;

        for (int i = 0; i < (int) normPop.size(); i++) {
            sfitt += normPop[i].first;
            normPop[i].first = sfitt;
        }

        std::cout << "Selected positions: ";
        for (int i = 0; i < left; i++) {
            double random = gekon::random(0, 1);
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
        unsigned int prev_gen_size = (unsigned int) prev_population.size();

        if (prev_gen_size / 2 % 2 == 0)
            new_gen_size = prev_gen_size / 2;
        else
            new_gen_size = prev_gen_size / 2 + 1;

        population_t new_gen;
        new_gen.resize(new_gen_size);

        for (unsigned int j = 0; j < new_gen_size; ++j) {
            unsigned int fst = (unsigned int) floor(random(0, prev_gen_size)),
                    snd = (unsigned int) floor(random(0, prev_gen_size));
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
            mod_pop[j].first = double(pop_size - j);
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

    static inline double fitness_vec(std::vector <tr_sample_t> samples,
                                     candidate_t candidate,
                                     fitness_fcn_t fit_fcn) {
        double ret_value = 0;
        std::for_each(samples.begin(), samples.end(), [&](auto &iter) {
            ret_value += fit_fcn(iter, candidate);
        });
        ret_value /= double(samples.size());
        return ret_value;
    }

    void fitness(fitness_fcn_t fit_fcn,
                 std::vector <tr_sample_t> samples,
                 population_t &generation,
                 const unsigned int threads_num) {
        size_t vec_size = generation.size();
        unsigned int num_of_threads = threads_num;
        std::vector <size_t> boundaries;

        unsigned int step = (unsigned int) vec_size / num_of_threads;
        for (unsigned int i = 0; i < num_of_threads; ++i) {
            //fixme: last thread has biggest portion
            boundaries.push_back(i * step);
        }
        boundaries.push_back(vec_size);
        std::thread *tt = new std::thread[num_of_threads];

        for (unsigned int i = 0; i < num_of_threads; ++i) {
            tt[i] = std::thread([&](size_t low, size_t high) {
                for (size_t j = low; j < high; ++j) {
                    //generation[j].first = fit_fcn(sample, generation[j].second);
                    generation[j].first = fitness_vec(samples, generation[j].second, fit_fcn);
                }
            }, boundaries[i], boundaries[i + 1]);
        }
        //fixme: main thread is idle
        for (unsigned int i = 0; i < num_of_threads; ++i)
            tt[i].join();

        delete[] tt;
    }

    std::vector <tr_sample_t> load_samples(std::string orig, std::string mod) {
        Mat_ <ker_num_t> mod_img;
        Mat_ <ker_num_t> orig_img;

        Mat imod = cv::imread(mod, cv::IMREAD_UNCHANGED);
        Mat iorig = cv::imread(orig, cv::IMREAD_UNCHANGED);

        if (!iorig.data || !imod.data) {
            std::cout << "Error while reading training samples." << std::endl;
            exit(-1);
        }

        if (imod.channels() != iorig.channels()) {
            std::cerr << "Original and modified images need to have the same number of channels!" << std::endl;
            exit(-1);
        }

        if (imod.channels() == 3) {
            Mat bgr_mod_type[3];
            split(imod, bgr_mod_type);
            Mat bgr_orig_type[3];
            split(iorig, bgr_orig_type);
            Mat bgr_orig[3], bgr_mod[3];
            for (int i = 0; i < 3; ++i) {
                bgr_orig_type[i].convertTo(bgr_orig[i], KERNEL_TYPE, 1 / 255.0);
                bgr_mod_type[i].convertTo(bgr_mod[i], KERNEL_TYPE, 1 / 255.0);
            }
            return {{bgr_orig[0], bgr_mod[0]},
                    {bgr_orig[1], bgr_mod[1]},
                    {bgr_orig[2], bgr_mod[2]}};
        } else if (imod.channels() == 1) {
            imod.convertTo(mod_img, KERNEL_TYPE, 1 / 255.0);
            iorig.convertTo(orig_img, KERNEL_TYPE, 1 / 255.0);
            return {{orig_img, mod_img}};
        } else {
            std::cerr << "Unknown image format with " << imod.channels() << " channels!" << std::endl;
            exit(-1);
        }

    }

    cv::Mat vec2image(std::vector <tr_sample_t> samples, candidate_t kernel) {
        if (samples.size() == 1) {
            Mat conv_result;
            cv::filter2D(samples[0].original, conv_result, -1, kernel, cv::Point(-1, -1), 0, cv::BORDER_CONSTANT);
            return conv_result;
        } else if (samples.size() == 3) {
            std::vector <Mat> img_vec = {
                    samples[0].original,
                    samples[1].original,
                    samples[2].original
            };
            Mat image;
            Mat conv_result;
            cv::merge(img_vec, image);
            cv::filter2D(image, conv_result, -1, kernel, cv::Point(-1, -1), 0, cv::BORDER_CONSTANT);
            return conv_result;
        } else {
            std::cerr << "Cannot construct result image. Wrong number of channels!" << std::endl;
            exit(-1);
        }

    }

    double fitness_ssim(const tr_sample_t sample, const candidate_t candidate) {

        double C1 = 6.5025, C2 = 58.5225;

        IplImage
                *img1 = NULL, *img2 = NULL, *img1_img2 = NULL,
                *img1_temp = NULL, *img2_temp = NULL,
                *img1_sq = NULL, *img2_sq = NULL,
                *mu1 = NULL, *mu2 = NULL,
                *mu1_sq = NULL, *mu2_sq = NULL, *mu1_mu2 = NULL,
                *sigma1_sq = NULL, *sigma2_sq = NULL, *sigma12 = NULL,
                *ssim_map = NULL, *temp1 = NULL, *temp2 = NULL, *temp3 = NULL;


        /***************************** INITS **********************************/

        Mat conv_result;
        filter2D(sample.original, conv_result, -1, candidate, cv::Point(-1, -1), 0, cv::BORDER_CONSTANT);

        CvMat img1_mat = CvMat(conv_result);
        CvMat img2_mat = CvMat(sample.modified);

        img1_temp = cvCreateImage(cvSize(img1_mat.width, img1_mat.height), IPL_DEPTH_8U, 1);
        img2_temp = cvCreateImage(cvSize(img2_mat.width, img2_mat.height), IPL_DEPTH_8U, 1);

        cvConvertImage(&img1_mat, img1_temp);
        cvConvertImage(&img2_mat, img2_temp);

        if (img1_temp == NULL || img2_temp == NULL)
            return -1;

        int x = img1_temp->width, y = img1_temp->height;
        int nChan = img1_temp->nChannels, d = IPL_DEPTH_32F;
        CvSize size = cvSize(x, y);

        img1 = cvCreateImage(size, d, nChan);
        img2 = cvCreateImage(size, d, nChan);

        cvConvert(img1_temp, img1);
        cvConvert(img2_temp, img2);

        img1_sq = cvCreateImage(size, d, nChan);
        img2_sq = cvCreateImage(size, d, nChan);
        img1_img2 = cvCreateImage(size, d, nChan);

        cvPow(img1, img1_sq, 2);
        cvPow(img2, img2_sq, 2);
        cvMul(img1, img2, img1_img2, 1);

        mu1 = cvCreateImage(size, d, nChan);
        mu2 = cvCreateImage(size, d, nChan);

        mu1_sq = cvCreateImage(size, d, nChan);
        mu2_sq = cvCreateImage(size, d, nChan);
        mu1_mu2 = cvCreateImage(size, d, nChan);


        sigma1_sq = cvCreateImage(size, d, nChan);
        sigma2_sq = cvCreateImage(size, d, nChan);
        sigma12 = cvCreateImage(size, d, nChan);

        temp1 = cvCreateImage(size, d, nChan);
        temp2 = cvCreateImage(size, d, nChan);
        temp3 = cvCreateImage(size, d, nChan);

        ssim_map = cvCreateImage(size, d, nChan);
        /*************************** END INITS **********************************/


        //////////////////////////////////////////////////////////////////////////
        // PRELIMINARY COMPUTING
        cvSmooth(img1, mu1, CV_GAUSSIAN, 11, 11, 1.5);
        cvSmooth(img2, mu2, CV_GAUSSIAN, 11, 11, 1.5);

        cvPow(mu1, mu1_sq, 2);
        cvPow(mu2, mu2_sq, 2);
        cvMul(mu1, mu2, mu1_mu2, 1);


        cvSmooth(img1_sq, sigma1_sq, CV_GAUSSIAN, 11, 11, 1.5);
        cvAddWeighted(sigma1_sq, 1, mu1_sq, -1, 0, sigma1_sq);

        cvSmooth(img2_sq, sigma2_sq, CV_GAUSSIAN, 11, 11, 1.5);
        cvAddWeighted(sigma2_sq, 1, mu2_sq, -1, 0, sigma2_sq);

        cvSmooth(img1_img2, sigma12, CV_GAUSSIAN, 11, 11, 1.5);
        cvAddWeighted(sigma12, 1, mu1_mu2, -1, 0, sigma12);


        //////////////////////////////////////////////////////////////////////////
        // FORMULA

        // (2*mu1_mu2 + C1)
        cvScale(mu1_mu2, temp1, 2);
        cvAddS(temp1, cvScalarAll(C1), temp1);

        // (2*sigma12 + C2)
        cvScale(sigma12, temp2, 2);
        cvAddS(temp2, cvScalarAll(C2), temp2);

        // ((2*mu1_mu2 + C1).*(2*sigma12 + C2))
        cvMul(temp1, temp2, temp3, 1);

        // (mu1_sq + mu2_sq + C1)
        cvAdd(mu1_sq, mu2_sq, temp1);
        cvAddS(temp1, cvScalarAll(C1), temp1);

        // (sigma1_sq + sigma2_sq + C2)
        cvAdd(sigma1_sq, sigma2_sq, temp2);
        cvAddS(temp2, cvScalarAll(C2), temp2);

        // ((mu1_sq + mu2_sq + C1).*(sigma1_sq + sigma2_sq + C2))
        cvMul(temp1, temp2, temp1, 1);

        // ((2*mu1_mu2 + C1).*(2*sigma12 + C2))./((mu1_sq + mu2_sq + C1).*(sigma1_sq + sigma2_sq + C2))
        cvDiv(temp3, temp1, ssim_map, 1);


        CvScalar index_scalar = cvAvg(ssim_map);

        // through observation, there is approximately 
        // 1% error max with the original matlab program

        // if you use this code within a program
        // don't forget to release the IplImages
        cvReleaseImage(&img1);
        cvReleaseImage(&img2);
        cvReleaseImage(&img1_img2);
        cvReleaseImage(&img1_temp);
        cvReleaseImage(&img2_temp);
        cvReleaseImage(&img1_sq);
        cvReleaseImage(&img2_sq);
        cvReleaseImage(&mu1);
        cvReleaseImage(&mu2);
        cvReleaseImage(&mu1_sq);
        cvReleaseImage(&mu2_sq);
        cvReleaseImage(&mu1_mu2);
        cvReleaseImage(&sigma1_sq);
        cvReleaseImage(&sigma2_sq);
        cvReleaseImage(&sigma12);
        cvReleaseImage(&ssim_map);
        cvReleaseImage(&temp1);
        cvReleaseImage(&temp2);
        cvReleaseImage(&temp3);
        return index_scalar.val[0] * 100;
    }
}
