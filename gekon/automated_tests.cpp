//
// Created by sehny on 3/26/16.
//

#include "cpptoml.h"
#include "gk_functions.h"
#include "gk_Worker.h"

#include <iostream>

using std::cout; using std::endl;
using std::cerr;

static const std::map<std::string, gekon::selection_fcn_t> selection_map = {
        {"roulette", gekon::s_roulette},
        {"rank selection", gekon::s_rank_selection},
        {"tournament", gekon::s_tournament}
};

class test_definition{
public:
    gekon::Worker worker;
    std::string name, selection, crossover, mutation;
    unsigned int number_of_threads, kernel_size, generation_size;

    test_definition() {
        name = selection = crossover = mutation = "default";
        number_of_threads = 4;
        kernel_size = 3;
        generation_size = 100;
    }
};

int main(int argc, char **argv)
{
    if (argc < 2) {
        cout << "Usage: ." << argv[0] << " <test file>" << endl;
        return 1;
    }

    std::shared_ptr<cpptoml::table> test_file = cpptoml::parse_file(argv[1]);
    std::vector<std::shared_ptr<cpptoml::table>> test_cases;
    std::vector<struct test_definition> workers;

    try
    {
        cpptoml::string_to_base_map::iterator it = test_file->begin();
        while (it != test_file->end()) {
            if (test_file->get(it->first)->is_table()) {
                test_cases.push_back(test_file->get_table(it->first));
                struct test_definition new_test;
                new_test.name = it->first;
                workers.push_back(new_test);
            }
            ++it;
        }
        //for (auto it_table : test_cases) {
        auto it_table = test_cases.begin();
        auto it_worker = workers.begin();
        while (it_table != test_cases.end() && it_worker != workers.end())
        {
            //std::cout << "# Test definition:" << std::endl;
            if ((*it_table)->contains("threads")) {
                const int64_t threads = (*it_table)->get("threads")->as<int64_t>()->get();
                it_worker->number_of_threads = (unsigned int)threads;
                it_worker->worker.overrideNumOfThreads((unsigned int)threads);
                //std::cout << "Number of threads: " << threads << std::endl;
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
                //std::cout << "Selection algorithm: " << selection << std::endl;
            }
            if ((*it_table)->contains("crossover")) {
                const std::string crossover = (*it_table)->get("crossove")->as<std::string>()->get();
                //std::cout << "Crossover algorithm: " << crossover<< std::endl;
            }
            if ((*it_table)->contains("mutation")) {
                const std::string mutation = (*it_table)->get("mutation")->as<std::string>()->get();
                //std::cout << "Mutation algorithm: " << mutation << std::endl;
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
    while (it_worker != workers.end()) {
        test_output("Test name: ", it_worker->name);
        test_output("Number of threads: ", std::to_string(it_worker->number_of_threads));
        test_output("Selection type: ", it_worker->selection);
        cout << test_tag << "Run!" << endl;
        time_t start, end;
        time(&start);
        time(&end);
        cout << test_tag << "Time elapsed: " << difftime(end, start) << endl;

        ++it_worker;
    }

    return 0;
}