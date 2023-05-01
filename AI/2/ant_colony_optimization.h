#pragma once

#include <vector>
#include "bin.h"

class AntColonyOptimization {
public:
    AntColonyOptimization(const std::vector<Bin> &items, int capacity, int num_ants, int num_iterations, double alpha, double beta, double rho, double q);

    int solve(int iteration, int total_iterations);
    void print_progress_bar(double progress);

private:
    const std::vector<Bin> &items;
    int capacity;
    int num_ants;
    int num_iterations;
    double alpha;
    double beta;
    double rho;
    double q;

    std::vector<std::vector<double>> pheromone_trails_;
    std::vector<std::vector<double>> heuristic_information_;

    void initialize_pheromone_trails();
    void initialize_heuristic_information();
    int construct_solution();
    void update_pheromone_trails(int best_value);
};
