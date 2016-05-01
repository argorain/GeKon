//
// Created by sehny on 3/26/16.
//

#include "cpptoml.h"
#include "gk_functions.h"
#include "gk_Worker.h"

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgcodecs/imgcodecs_c.h>

using std::cout; using std::endl;
using std::cerr;

using namespace gekon;

static const std::map<std::string, gekon::selection_fcn_t> selection_map = {
        {"roulette", gekon::s_roulette},
        {"rank selection", gekon::s_rank_selection},
        {"tournament", gekon::s_tournament}
};

static const std::map<std::string, gekon::crossover_fcn_t> crossover_map = {
        {"simple", gekon::c_simle},
        {"convex", gekon::c_convex},
        {"blx_a", gekon::c_blx_a}
};

static const std::map<std::string, gekon::mutation_fcn_t> mutation_map = {
        {"swap", gekon::m_swap},
        {"dynamic", gekon::m_dynamic}
};

static const std::map<std::string, gekon::fitness_fcn_t> fitness_map = {
        {"mse", gekon::fitness_mse},
        {"ssim", gekon::fitness_ssim}
};

class test_definition{
public:
    gekon::Worker worker;
    std::string name, selection, crossover, mutation, fitness;
    unsigned int number_of_threads, kernel_size, generation_size, max_iterations;

    test_definition() {
        name = selection = crossover = mutation = fitness = "default";
        number_of_threads = 4;
        kernel_size = 3;
        generation_size = 80;
        max_iterations = 500;
    }
};

int main(int argc, char **argv)
{
    if (argc < 4) {
        cout << "Usage: ." << argv[0] << " <test file> <original> <modified>" << endl;
        return 1;
    }

    std::shared_ptr<cpptoml::table> test_file = cpptoml::parse_file(argv[1]);
    std::vector<std::shared_ptr<cpptoml::table>> test_cases;
    std::vector<test_definition> workers;

    try
    {
        cpptoml::string_to_base_map::iterator it = test_file->begin();
        while (it != test_file->end()) {
            if (test_file->get(it->first)->is_table()) {
                test_cases.push_back(test_file->get_table(it->first));
                test_definition new_test;
                new_test.name = it->first;
                workers.push_back(new_test);
            }
            ++it;
        }
        auto it_table = test_cases.begin();
        auto it_worker = workers.begin();
        while (it_table != test_cases.end() && it_worker != workers.end())
        {
            if ((*it_table)->contains("threads")) {
                const int64_t threads = (*it_table)->get("threads")->as<int64_t>()->get();
                it_worker->number_of_threads = (unsigned int)threads;
                it_worker->worker.overrideNumOfThreads((unsigned int)threads);
            }
            if ((*it_table)->contains("kernel_size")) {
                const int64_t kernel_size = (*it_table)->get("kernel_size")->as<int64_t>()->get();
                it_worker->kernel_size = (unsigned int)kernel_size;
                it_worker->worker.setKernelSize((unsigned int)kernel_size);
            }
            if ((*it_table)->contains("generation_size")) {
                const int64_t generation_size = (*it_table)->get("generation_size")->as<int64_t>()->get();
                it_worker->generation_size = (unsigned int)generation_size;
                it_worker->worker.setGenSize((unsigned int)generation_size);
            }
            if ((*it_table)->contains("max_iterations")) {
                const int64_t max_iterations = (*it_table)->get("max_iterations")->as<int64_t>()->get();
                it_worker->max_iterations = (unsigned int)max_iterations;
            }
            if ((*it_table)->contains("selection")) {
                const std::string selection = (*it_table)->get("selection")->as<std::string>()->get();
                it_worker->selection = selection;
                auto search = selection_map.find(selection);
                if (search != selection_map.end()) {
                    it_worker->worker.setSelectionFcn(search->second);
                } else {
                    cerr << "Selection type not found!" << endl;
                }
            }
            if ((*it_table)->contains("crossover")) {
                const std::string crossover = (*it_table)->get("crossover")->as<std::string>()->get();
                it_worker->crossover = crossover;
                auto search = crossover_map.find(crossover);
                if (search != crossover_map.end()) {
                    it_worker->worker.setCrossoverFcn(search->second);
                } else {
                    cerr << "Crossover type not found!" << endl;
                }
            }
            if ((*it_table)->contains("mutation")) {
                const std::string mutation = (*it_table)->get("mutation")->as<std::string>()->get();
                it_worker->mutation = mutation;
                auto search = mutation_map.find(mutation);
                if (search != mutation_map.end()) {
                    it_worker->worker.setMutationFcn(search->second);
                } else {
                    cerr << "Mutation type not found!" << endl;
                }
            }
            if ((*it_table)->contains("fitness")) {
                const std::string fitness = (*it_table)->get("fitness")->as<std::string>()->get();
                it_worker->fitness = fitness;
                auto search = fitness_map.find(fitness);
                if (search != fitness_map.end()) {
                    it_worker->worker.setFitnessFcn(search->second);
                } else {
                    cerr << "Fitness type not found!" << endl;
                }
            }
            it_table++; it_worker++;
        }
    }
    catch (const cpptoml::parse_exception& e)
    {
        std::cerr << "Failed to parse " << argv[1] << ": " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Running automated tests" << std::endl;

    auto it_worker = workers.begin();
    const std::string test_tag = "[TEST]";
    auto test_output = [&](std::string comment, std::string value){
        cout << test_tag << comment << value << endl;
    };

    std::string original {argv[2]};
    std::string modified {argv[3]};
    auto samples = load_samples(original, modified);

    srand ((unsigned int)time(NULL));
    cv::theRNG().state = (uint64_t)time(NULL); //random seed for opencv. Need to be initialized for each thread.

    while (it_worker != workers.end()) {
        test_output("Test name: ", it_worker->name);
        test_output("Number of threads: ", std::to_string(it_worker->number_of_threads));
        test_output("Max. number of iterations: ", std::to_string(it_worker->max_iterations));
        test_output("Selection type: ", it_worker->selection);
        test_output("Crossover type: ", it_worker->crossover);
        test_output("Mutation type: ", it_worker->mutation);
        test_output("Kernel size: ", std::to_string(it_worker->kernel_size));
        test_output("Generation size: ", std::to_string(it_worker->generation_size));
        test_output("Fitness funtion: ", it_worker->fitness);
        cout << test_tag << "Run!" << endl;

        it_worker->worker.setTrSamples(samples);

        time_t start, end;
        time(&start);
        unsigned int loops = it_worker->max_iterations;
        std::vector<double> elite;
        elite.resize(loops);
        for (unsigned int j = 0; j < loops; ++j) {
            auto ret = it_worker->worker.run();
            elite.push_back(ret);
            cout << j << "/" << loops << endl;
        }
        cout << test_tag << "Elite: [";
        for (unsigned int i = 0; i < elite.size() - 1; ++i) {
            cout << elite[i] << ", ";
        }
        cout << elite[elite.size()-1]  << "]" << endl;

        time(&end);
        cout << test_tag << "Time elapsed: " << difftime(end, start) << endl;

        time_t seconds;
        time(&seconds);


        cout << "output/"+it_worker->name+"_"+basename(argv[2])+"_"+std::to_string(seconds)+".txt" << endl;

        std::ofstream output;
        auto sol = it_worker->worker.retBestSolution();
        output.open("output/"+it_worker->name+"_"+basename(argv[2])+"_"+std::to_string(seconds)+".txt");
        output << sol << endl;
        output.close();

        auto img = vec2image(samples, sol);
        cv::imwrite("output/"+it_worker->name+"_"+basename(argv[2])+"_"+std::to_string(seconds)+".jpg", img*255);

        ++it_worker;
    }

    return 0;
}
