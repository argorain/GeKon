#ifndef BOOST_ALL_DYN_LINK
#define BOOST_ALL_DYN_LINK
#endif

#include <stdexcept>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/algorithm/string.hpp>

#include "gnuplot-iostream.h"

#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"

#include "gk_utils.h"

using namespace std;
using namespace cv;
namespace fs=boost::filesystem;

namespace gekon {
    tr_sample_t load_img_pair(const std::string path, const std::string name) {

        tr_sample_t sample = tr_sample_t();

        cout << "Loading image pair "
        << path + name + org_suff + suff << " and "
        << path + name + mod_suff + suff << "... ";

        sample.original = imread(path + name + org_suff + suff, 1);
        if (sample.original.empty()) {
            cout << endl;
            throw runtime_error("Original image " + path + name + org_suff + suff + " is unreadable or not found.");
        }

        sample.modified = imread(path + name + mod_suff + suff, 1);
        if (sample.modified.empty()) {
            cout << endl;
            throw runtime_error("Modified image " + path + name + mod_suff + suff + " is unreadable or not found.");
        }

        cout << "Done." << endl;
        return sample;
    }

    std::vector <tr_sample_t> load_samples(const std::string dir_name) {
        vector <tr_sample_t> samples;
        fs::path directory(dir_name);
        if (!fs::is_directory(directory)) {
            cout << dir_name << " is not a directory!" << endl;
            assert(false);
        }

        for (auto &f : boost::make_iterator_range(fs::directory_iterator(directory), {})) {
            std::string filename = f.path().string();
            std::vector <std::string> strs;

            // split address by '.' and '/'
            boost::split(strs, filename, boost::is_any_of("./"));

            if (suff.compare("." + strs[strs.size() - 1]) == 0) {
                // suffix match, continue
                if (mod_suff.compare("." + strs[strs.size() - 2]) == 0) {
                    // modified suffix match, continue
                    std::string name = strs[strs.size() - 3];
                    std::string path = filename.substr(0,
                                                       filename.size() - suff.size() - mod_suff.size() - name.size());
                    try {
                        // we expect existence of original when modified version found.
                        samples.push_back(load_img_pair(path, name));
                    } catch (exception &e) {
                        // if not, never mind, just inform user.
                        cerr << "[ERROR] Matching original image not found. " << e.what() << endl;
                    }
                }
            }
        }

        return samples;
    }


    void plot_graph(const std::vector<double> points, std::string name) {
        Gnuplot gp;

        gp << "plot" << gp.file1d(points) << "with lines title '" << name << "'\n";
    }
}
