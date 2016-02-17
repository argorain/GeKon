#include <iostream>
#include <unistd.h>

#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

static void help()
{
    cout << endl << "Usage: ./gekon lena.jpg" << endl;
}

const char* keys =
{
    "{help h||}{@image |lena.jpg|input image name}"
};

int main(int argc, char **argv) {
	
	cout << "GeKon" << endl;
	cout << "Vojtech Vladyka & Martin Sehnoutka" << endl; 
	cout << "FEEC BUT 2016" << endl;
	cout << "******************" << endl;

	// do something here

	
	Mat image;

    CommandLineParser parser(argc, argv, keys);
    if (parser.has("help"))
    {
        help();
        return 0;
    }
    string filename = parser.get<string>(0);
    image = imread(filename, 1);
    if(image.empty())
    {
        printf("Cannot read image file: %s\n", filename.c_str());
        help();
        return -1;
    }
    
	// Show the image
    imshow("Original Image", image);
    // Wait for a key stroke; the same function arranges events processing
    waitKey(0);
	
	cout << "Bye!" << endl;
	return 0;
}
