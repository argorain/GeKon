#include <iostream>
#include <unistd.h>

#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include "gk_utils.h"
#include "gk_types.h"
#include "gk_functions.h"
#include "gk_operators.h"
#include "gk_Worker.h"

using std::cout; using std::endl; using std::string;
using namespace cv;
//using cv::CommandLineParser; using cv::Mat;
using namespace gekon;

static void help()
{
    cout << "Usage: gekon [PARAMETER] [ORIGNAL IMAGE] [MODIFIED IMAGE]" << endl << endl;
    cout << "All parameters are mandatory." << endl;
    cout << "PARAMETER is size of convolution matrix" << endl;
    cout << "ORIGINAL IMAGE is image before modification" << endl;
    cout << "MODIFIED IMAGE is image after modification" << endl << endl;
    cout << "Example of usage: ./gekon 3 lena.jpg lena-mod.jpg" << endl << endl;
    cout << "Authors: Vojtech Vladyka <vojtech.vladyka@gmail.com> and Martin Sehnoutka <>" << endl;
    cout << "Department of Automation and Measurement, FEEC BUT, 2016" << endl;
}

const char* keys =
{
    "{help h||}{@convSize|3|convolution size}{@original|lena.jpg|original image name}{@modified|lena.mod.jpg|modiifed image name}"
};

int main(int argc, char **argv) {
    srand ((unsigned int)time(NULL));

    cout << "GeKon" << endl;

    CommandLineParser parser(argc, argv, keys);
    if (parser.has("help"))
    {
        help();
        return 0;
    }
    string original = parser.get<string>(1);
    string modified = parser.get<string>(2);
    int convSize = parser.get<int>(0);    	

    cout << "Convolution matrix size: " << convSize << endl;
    cout << "Original image: " << original << endl;
    cout << "Modified image: " << modified << endl;

/*    Mat_<ker_num_t > mod_img;
    Mat i1 = imread(modified, CV_LOAD_IMAGE_GRAYSCALE);
    i1.convertTo(mod_img, KERNEL_TYPE, 1/255.0);
    Mat_<ker_num_t > orig_img;
    Mat i2 = imread(original, CV_LOAD_IMAGE_GRAYSCALE);
    i2.convertTo(orig_img, KERNEL_TYPE, 1/255.0);

    imshow("Image", i1);
    cv::waitKey(0);
    imshow("Image", i2);
    cv::waitKey(0);
    imshow("Image", mod_img);
    cv::waitKey(0);
    imshow("Image", orig_img);
    cv::waitKey(0);
*/


    // init gekon here

    cv::theRNG().state = time(NULL); //random seed for opencv. Need to be initialized for each thread.

/*    tr_sample_t sample = {
            orig_img,
            mod_img
    };*/
    auto samples = load_samples(original, modified);
    Worker the_gekon;//80,convSize);
    the_gekon.setGenSize(80);
    the_gekon.setKernelSize(convSize);
    the_gekon.setTrSamples(samples);
    the_gekon.setSelectionFcn(s_tournament);
    the_gekon.setMutationFcn(m_dynamic);

    std::vector<double> fitPlot;

    // test inputs

    // run!
    time_t start, end;
    time(&start);
    int loops = 20;
    for (int j = 0; j < loops; ++j) {
        auto ret = the_gekon.run();
        cout << endl << "Elite: " << ret << endl
             << "Iterations: " << j << "/" << loops << endl << endl;
        fitPlot.push_back(ret);
    }
    time(&end);
    cout << "Time elapsed: " << difftime(end, start) << endl;
    plot_graph(fitPlot, "Fitness");
    
    auto sol = the_gekon.retBestSolution();

    cout << sol << endl;

    auto img = vec2image(samples, sol);
    namedWindow("Result", CV_WINDOW_AUTOSIZE);
    imshow("Result", img);
    cv::waitKey(0);

    /*
    Mat conv_result;
    filter2D(sample.original, conv_result, -1, sol, cv::Point(-1, -1), 0, cv::BORDER_CONSTANT);

    auto w_sol = the_gekon.retWorstSolution();

    cout << w_sol << endl;

    Mat conv_result2;
    filter2D(sample.original, conv_result2, -1, w_sol, cv::Point(-1, -1), 0, cv::BORDER_CONSTANT);
    imshow("Image", sample.modified);
    cv::waitKey(0);
    imshow("Image", conv_result);
    cv::waitKey(0);
    imshow("Image", conv_result2);
    cv::waitKey(0);
*/
    cout << "Bye!" << endl;
    return 0;
}
