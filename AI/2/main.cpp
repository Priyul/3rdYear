#include <iostream> //input and output 
#include <fstream> //file stream
#include <vector> //vector data structure
#include <string> 
#include <algorithm>
#include <map> //map data structure
#include <ctime> // time elapawa
#include <cstdlib>
#include <sstream> // for reading from the excel file
#include <chrono> // for time elapsed
#include <thread>
#include <mutex>
#include <random> //for random selection
#include <bitset> // main functionality
#include <deque> // for tabu search

#include "bin.h"
#include "ant_colony_optimization.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif

using namespace std;
std::map<std::string, int> known_optimal_solutions;

void print_items(const std::vector<Bin> &items) {
    std::cout << "Items:" << std::endl;
    for (const auto &item : items) {
        std::cout << "Value: " << item.value << ", Weight: " << item.weight << std::endl;
    }
}

void print_progress_bar(double progress) {
    int bar_width = 50;

    std::cout << "[";
    int position = static_cast<int>(bar_width * progress);
    for (int i = 0; i < bar_width; ++i) {
        if (i < position)
            std::cout << "=";
        else if (i == position)
            std::cout << ">";
        else
            std::cout << " ";
    }
    std::cout << "] " << static_cast<int>(progress * 100.0) << " %\r";
    std::cout.flush();
}

void solve_knapsack_problem(const std::vector<Bin> &items, int capacity, int iteration, int total_iterations) {

    int num_ants = 10; // Number of ants
    int num_iterations = 100; // Number of iterations
    double alpha = 1.0; // Pheromone importance
    double beta = 2.0; // Greedy heuristic importance
    double rho = 0.5; // Pheromone evaporation rate
    double q = 100.0; // Pheromone update constant

    AntColonyOptimization aco(items, capacity, num_ants, num_iterations, alpha, beta, rho, q);
    int best_value = aco.solve(iteration, total_iterations);

    std::cout << "Best value: " << best_value << std::endl;
}


string fileNames[11] = {"KnapsackInstances/f1_l-d_kp_10_269.txt", "KnapsackInstances/f2_l-d_kp_20_878.txt", "KnapsackInstances/f3_l-d_kp_4_20.txt", "KnapsackInstances/f4_l-d_kp_4_11.txt",
 "KnapsackInstances/f5_l-d_kp_15_375.txt", "KnapsackInstances/f6_l-d_kp_10_60.txt", "KnapsackInstances/f7_l-d_kp_7_50.txt", "KnapsackInstances/f8_l-d_kp_23_10000.txt", 
 "KnapsackInstances/f9_l-d_kp_5_80.txt", "KnapsackInstances/f10_l-d_kp_20_879.txt", "KnapsackInstances/knapPI_1_100_1000_1.txt"};

int main() {
    for (int file_num = 0; file_num < 11; ++file_num) {
        std::string input_file = fileNames[file_num];
        std::ifstream file(input_file);

        if (!file.is_open()) {
            std::cerr << "Error opening file: " << input_file << std::endl;
            return 1;
        }

        int num_items, capacity;
        file >> num_items >> capacity;

        std::vector<Bin> items;

        for (int i = 0; i < num_items; ++i) {
            int value, weight;
            file >> value >> weight;
            items.emplace_back(value, weight);
        }

        file.close();

        cout << "File: " << fileNames[file_num] << endl;
        // print_items(items);
        // cout << "________________________________"  << endl;

        // Now, items and capacity are available for the ant colony search algorithm.
        solve_knapsack_problem(items, capacity, file_num, 11);
    }

    return 0;
}
