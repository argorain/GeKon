#include <iostream>
#include <unistd.h>

#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include "gk_utils.h"

using namespace std;
using namespace cv;

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

	// init gekon here

	// test inputs

	// run!

	cout << "Bye!" << endl;
	return 0;
}
