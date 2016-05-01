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

    // init gekon here

    cv::theRNG().state = time(NULL); //random seed for opencv. Need to be initialized for each thread.

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
    int loops = 1000; // maximum iterations
    float goodEnough = 85; //ending condition
    for (int j = 0; j < loops; ++j) {
        auto ret = the_gekon.run();
        cout << endl << "Elite: " << ret << endl
             << "Iterations: " << j << "/" << loops << endl << endl;
        fitPlot.push_back(ret);
        if(ret > goodEnough) {
            cout << "Found solution good enough. Ending." << endl;
            break;
        }
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

    cout << "Bye!" << endl;
    return 0;
}
