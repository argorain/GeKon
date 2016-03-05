//
// Created by sehny on 3/2/16.
//

#include <iostream>

#include "gk_utils.h"

using namespace std;
using namespace gekon;

int main() {

	//plot playground
	std::vector<double> data;
	for(int i=0; i<200; i++) {
		data.push_back(1/(double)i + 2);
	}
	plot_graph(data, "fitness");

	//load samples playground
    load_samples("test_samples/");
}
