
#include <stdexcept>

#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"

#include "gk_utils.h"

using namespace std;
using namespace gekon;
using namespace cv;


tr_sample_t* load_img_pair(const std::string original, const std::string modified) {

	tr_sample_t *sample = (tr_sample_t*)malloc(sizeof(tr_sample_t*));	

    sample->original = imread(original, 1);
    if(sample->original.empty()) {
		throw runtime_error("Original image is unreadable.");
    }

	sample->modified = imread(modified, 1);
	if(sample->modified.empty()) {
		throw runtime_error("Modified image is unreadable.");
	}
	 

	return sample;	
}
