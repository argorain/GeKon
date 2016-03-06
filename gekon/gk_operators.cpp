
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
	
	float random(float min, float max) {
		return min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
	}

	std::vector<candidate_t> blx_a(candidate_t X, candidate_t Y) {
		// CONSTATNS
		const float alpha = 0.5;

		// ALGORITHM
		srand (static_cast <unsigned> (time(0)));

		int rows = X.rows;
		int cols = X.cols;
		candidate_t kidX = Mat::zeros(rows, cols, CV_32F);
		candidate_t kidY = Mat::zeros(rows, cols, CV_32F);

        for (int r = 0; r < rows; ++r) {
			for(int c = 0; c< cols; ++c) {
				float xt = X.at<float>(r,c);
				float yt = Y.at<float>(r,c);
				float diff = abs(xt-yt);
				//cout << (float)(diff.at<cv::Vec3b>(r,c)[0]) << " ";
				float min, max;
				if(xt > yt) {
					min = yt-diff*alpha;
				   	max = xt+diff*alpha;	
				} else {
					min = xt-diff*alpha;
					max = yt+diff*alpha;
				}
				//cout << "min: " << min << " max: " << max << endl;
				float rX = random(min, max);
				float rY = random(min, max); 
				//cout << "rX: " << rX << " rY: " << rY << endl;
				kidX.at<float>(r,c) = rX;	
				kidY.at<float>(r,c) = rY;
				//cout << "rXV: " << (float)(kidX.at<float>(r,c)) << endl;
			}
			//cout << endl;
		}		

		std::vector<candidate_t> kids;
		kids.push_back(kidX);
		kids.push_back(kidY);

		return kids;

	}

	candidate_t m_swap(candidate_t X) {
		//CONSTANTS
		const float m_probabilty = 0.99;
		
		
		//ALGORITHM
		srand (static_cast <unsigned> (time(0)));
	
		if(random(0,1) < m_probabilty) {
			// Mutate!
			int rows = X.rows;
			int cols = X.cols;
			int r1 = random(-0.4, rows-1+0.4); // that +-0.4 is to ensure whole interval
			int c1 = random(-0.4, cols-1+0.4);
			int r2 = random(-0.4, rows-1+0.4); 
			int c2 = random(-0.4, cols-1+0.4);
			cout << "Swap between [" << r1 << "," << c1 << "],[" << r2 << "," << c2 << "]" << endl;
			float g = X.at<float>(r1, c1);
			X.at<float>(r1, c1) = X.at<float>(r2, c2);
			X.at<float>(r2, c2) = g;
		}

		return X;
	}

}
