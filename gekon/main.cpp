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

    Mat mod_img = imread(modified, CV_LOAD_IMAGE_GRAYSCALE);
    Mat orig_img = imread(original, CV_LOAD_IMAGE_GRAYSCALE);

    if (!mod_img.data || !orig_img.data)
    {
        cout << "Error while reading training samples." << endl;
        return -1;
    }

    // init gekon here
    tr_sample_t sample = {
            .original = orig_img,
            .modified = mod_img
    };
    Worker the_gekon(20,convSize);
    the_gekon.setTrSample(sample);

    // test inputs

    // run!
    for (int j = 0; j < 10; ++j) {
        cout << the_gekon.run() << endl;
    }

    cout << "Bye!" << endl;
    return 0;
}
