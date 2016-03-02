
#include <stdexcept>
#include <experimental/filesystem>

#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"

#include "gk_utils.h"

using namespace std;
using namespace cv;
namespace fs=std::experimental::filesystem;

namespace gekon {
	tr_sample_t load_img_pair(const std::string name) {

		//tr_sample_t *sample = (tr_sample_t*)malloc(sizeof(tr_sample_t*));
		tr_sample_t sample = tr_sample_t();

		cout << "Loading image pair "
			<< name + org_suff << " and "
			<< name + mod_suff << "." << endl;

		sample.original = imread(name + org_suff, 1);
		if (sample.original.empty()) {
			throw runtime_error("Original image is unreadable.");
		}

		sample.modified = imread(name + mod_suff, 1);
		if (sample.modified.empty()) {
			throw runtime_error("Modified image is unreadable.");
		}


		return sample;
	}

	std::vector<tr_sample_t> load_samples (const std::string dir_name)
    {
        vector<tr_sample_t> samples;
        fs::path directory(dir_name);
        if (!fs::is_directory(directory)) {
            cout << dir_name << " is not a directory!" << endl;
            assert(false);
        }
        for (auto& f : fs::directory_iterator(directory)) {
            cout << f << endl;
        }
        return samples;
    }
}