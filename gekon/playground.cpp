//
// Created by sehny on 3/2/16.
//

#include <iostream>
#include <algorithm>

//#include "opencv2/core.hpp"
//#include "opencv2/core/utility.hpp"
//#include "opencv2/imgproc.hpp"
//#include "opencv2/imgcodecs.hpp"

//#include "gk_utils.h"
//#include "gk_operators.h"
//#include "gk_Worker.h"

#include "SSIM.h"

using namespace std;
//using namespace gekon;

int main() {

    //plot playground
    /*std::vector<double> data;
    for(int i=0; i<200; i++) {
        data.push_back(1/(double)i + 2);
    }
    plot_graph(data, "fitness");
    //load samples playground
    load_samples("test_samples/");

    //crosover
    //
    cout << " CROSSOVER " << endl;
    candidate_t X = cv::Mat::zeros(3, 3, CV_32F);
    candidate_t Y = cv::Mat::zeros(3, 3, CV_32F);
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            X.at<float>(i,j) = i*j+1;	
            Y.at<float>(i,j) = i+j*2;	
        }
    }
    std::vector<candidate_t> kids = c_blx_a(X,Y);

    cout << "X" << endl;
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            cout << (float)(X.at<float>(i,j)) << " ";
        }
        cout << endl;
    }
    cout << "Y" << endl;
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            cout << (float)(Y.at<float>(i,j)) << " ";
        }
        cout << endl;
    }
    cout << "X+1" << endl;
    candidate_t k = kids.back();
    kids.pop_back();
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            cout << (float)(k.at<float>(i,j)) << " ";
        }
        cout << endl;
    }
    cout << "Y+1" << endl;
    k=kids.back();
    k.pop_back();
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            cout << (float)(k.at<float>(i,j)) << " ";
        }
        cout << endl;
    }

    // mutate
    cout << " MUTATE " << endl;
    cout << "Y" << endl;
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            cout << (float)(Y.at<float>(i,j)) << " ";
        }
        cout << endl;
    }
    Y = m_swap(Y);

    cout << "Y" << endl;
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            cout << (float)(Y.at<float>(i,j)) << " ";
        }
        cout << endl;
    }

    cout << "FIRST GEN" << endl;

    population_t test_pop = first_generation(10, 3);
    for (auto &iter: test_pop) {
        cout << iter.first << ' ' << iter.second << endl;
    }

    cout << "ROULETTE TEST" << endl;
    double fit = 100;
    cout << "Fitness seed" << endl;
    for (auto &iter: test_pop) {
        iter.first = fit;
        fit -= 10; 
        cout << iter.first << ' ' << iter.second << endl;
    }

    std::sort(test_pop.begin(), test_pop.end(), cmp_candidates);

    cout << "Sorted..." << endl;
    for (auto &iter: test_pop) {
        cout << iter.first << ' ' << iter.second << endl;
    }

    test_pop = s_roulette(test_pop);
    cout << "Selected" << endl;
    for (auto &iter: test_pop) {
        cout << iter.first << ' ' << iter.second << endl;
    }


    cout << "WORKER TEST" << endl;
    Worker test();

    */

    ssim("samples/lena_gray.png", "samples/lena_gray_M.png");
    ssim("samples/lena_gray.png", "samples/lena_gray.mod.png");
    ssim("samples/lena_gray.png", "samples/lena_gray.png");


}
