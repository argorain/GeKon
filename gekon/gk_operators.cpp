
#ifndef BOOST_ALL_DYN_LINK
	#define BOOST_ALL_DYN_LINK
#endif 

#include <stdexcept>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/algorithm/string.hpp>

#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"

#include "gk_operators.h"

using namespace std;
using namespace cv;

namespace gekon {
	
	ker_num_t random(ker_num_t min, ker_num_t max) {
		auto rand_num = rand();
        auto ret = ker_num_t(float(min) + float(rand_num)/float(RAND_MAX/float(max-min)));
        return ret;
	}

	std::vector<candidate_t> c_blx_a(candidate_t X, candidate_t Y) {
		int rows = X.rows;
		int cols = X.cols;
		candidate_t kidX = Mat::zeros(rows, cols, KERNEL_TYPE);
		candidate_t kidY = Mat::zeros(rows, cols, KERNEL_TYPE);

        for (int r = 0; r < rows; ++r) {
			for(int c = 0; c< cols; ++c) {
				float xt = X.at<ker_num_t>(r,c);
				float yt = Y.at<ker_num_t>(r,c);
				float diff = abs(xt-yt);
				float min, max;
				if(xt > yt) {
					min = yt-diff*c_blx_a_alpha;
				   	max = xt+diff*c_blx_a_alpha;	
				} else {
					min = xt-diff*c_blx_a_alpha;
					max = yt+diff*c_blx_a_alpha;
				}
				float rX = random(min, max);
				float rY = random(min, max); 
				kidX.at<ker_num_t>(r,c) = rX;
				kidY.at<ker_num_t>(r,c) = rY;
			}
        }

		std::vector<candidate_t> kids;
		kids.push_back(kidX);
		kids.push_back(kidY);

		return kids;

	}

    std::vector<candidate_t> c_convex(candidate_t X, candidate_t Y) {
        // random vectors
        vector<ker_num_t> rand_vec;
        // new candidates
        vector<candidate_t> kids;
        int rows = X.rows, cols = X.cols;
        assert(X.rows == Y.rows && X.cols == Y.cols);
        // initialization of container types
        rand_vec.resize((unsigned int)rows*cols, 0);
        kids.resize(2, Mat::zeros(rows, cols, KERNEL_TYPE));

        for (int j = 0; j < 2; ++j) {
            for_each(rand_vec.begin(), rand_vec.end(), [](auto &iter) {
                return random(0, 1);
            });
            for (int k=0; k < rows*cols; ++k) {
                int r = k/cols;
                int c = k%cols;
                kids[j].at<ker_num_t>(r,c) = rand_vec[k]*X.at<ker_num_t>(r,c) + (1-rand_vec[k])* Y.at<ker_num_t>(r,c);
            }
        }

        return kids;
    }

    std::vector<candidate_t> c_simle(candidate_t X, candidate_t Y) {
        // random vectors
        vector<ker_num_t> rand_vec;
        vector<bool> decision_vec;
        // new candidates
        vector<candidate_t> kids;
        int rows = X.rows, cols = X.cols;
        assert(X.rows == Y.rows && X.cols == Y.cols);
        // initialization of container types
        rand_vec.resize((unsigned int)rows*cols, 0);
        decision_vec.resize((unsigned int)rows*cols, false);
        kids.resize(2, Mat::zeros(rows, cols, KERNEL_TYPE));

        for (int j = 0; j < 2; ++j) {
            for_each(rand_vec.begin(), rand_vec.end(), [](auto &iter) {
                return random(0, 1);
            });
            transform(rand_vec.begin(), rand_vec.end(), decision_vec.begin(), [](auto iter) {
                return iter > 0.5;
            });
            for (int k=0; k < rows*cols; ++k) {
                int r = k/cols;
                int c = k%cols;
                kids[j].at<ker_num_t>(r,c) = (decision_vec[k]) ? X.at<ker_num_t>(r,c) : Y.at<ker_num_t>(r,c);
            }
        }
        return kids;
    }

	void m_swap(candidate_t &X, unsigned int t, unsigned int T) {
        if(random(0,1) < m_swap_prob) {
			// Mutate!
			int rows = X.rows;
			int cols = X.cols;
			int r1 = random(-0.4, rows-1+0.4); // that +-0.4 is to ensure whole interval
			int c1 = random(-0.4, cols-1+0.4);
			int r2 = random(-0.4, rows-1+0.4); 
			int c2 = random(-0.4, cols-1+0.4);
			float g = X.at<float>(r1, c1);
			X.at<float>(r1, c1) = X.at<float>(r2, c2);
			X.at<float>(r2, c2) = g;
		}

	}

    void m_dynamic(candidate_t &X, unsigned int t, unsigned int T) {
        /*
         * Dynamic mutation (Michalewicz):
         * Defined as $\Delta(t,y) = y \left( 1 - r^{ {1-\frac{t}{T} }^B }  \right)$
         *     where B = 5
         */
        int rows = X.rows;
        int cols = X.cols;
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if(random(0,1) < m_swap_prob) {
                    X.at<ker_num_t>(r,c) = X.at<ker_num_t>(r,c)*(1-pow(random(0,1), pow(float(1)-float(t)/T, m_dynamic_B)));
                }
            }
        }
    }
}
