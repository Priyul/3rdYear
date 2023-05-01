#include "ant_colony_optimization.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <limits>
#include <random>

AntColonyOptimization::AntColonyOptimization(const std::vector<Bin> &items, double capacity, int num_ants, int num_iterations, double alpha, double beta, double rho, double q)
    : items(items), capacity(capacity), num_ants(num_ants), num_iterations(num_iterations), alpha(alpha), beta(beta), rho(rho), q(q) {
    initialize_pheromone_trails();
    initialize_heuristic_information();
}

void AntColonyOptimization::initialize_pheromone_trails() {
    pheromone_trails_.resize(items.size(), std::vector<double>(items.size(), 1.0));
}

void AntColonyOptimization::initialize_heuristic_information() {
    heuristic_information_.resize(items.size(), std::vector<double>(items.size()));

    for (size_t i = 0; i < items.size(); ++i) {
        for (size_t j = 0; j < items.size(); ++j) {
            heuristic_information_[i][j] = static_cast<double>(items[j].value) / items[j].weight;
        }
    }
}

void AntColonyOptimization::print_progress_bar(double progress) {
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

// int AntColonyOptimization::solve(int iteration, int total_iterations) {
//     int best_value = std::numeric_limits<int>::min();

//     for (int iter = 0; iter < num_iterations; ++iter) {
//         int iter_best_value = std::numeric_limits<int>::min();

//         for (int ant = 0; ant < num_ants; ++ant) {
//             int solution_value = construct_solution();
//             iter_best_value = std::max(iter_best_value, solution_value);
//         }

//         best_value = std::max(best_value, iter_best_value);
//         update_pheromone_trails(iter_best_value);
//         print_progress_bar(static_cast<double>(iter) / num_iterations);
//     }

//     return best_value;
// }

double AntColonyOptimization::construct_solution() {
    std::vector<bool> used_items(items.size(), false);
    double current_capacity = capacity;
    double current_value = 0;
    
    while (true) {
        double sum_prob = 0.0;
        std::vector<double> probabilities(items.size(), 0.0);
        
        for (size_t i = 0; i < items.size(); ++i) {
            if (!used_items[i] && items[i].weight <= current_capacity) {
                probabilities[i] = pow(pheromone_trails_[i][1], alpha) * pow(heuristic_information_[i][1], beta);
                sum_prob += probabilities[i];
            }
        }
        
        if (sum_prob == 0.0) {
            break;
        }
        
        for (size_t i = 0; i < items.size(); ++i) {
            probabilities[i] /= sum_prob;
        }
        
        std::discrete_distribution<size_t> dist(probabilities.begin(), probabilities.end());
        std::random_device rd;
        std::mt19937 gen(rd());
        size_t selected_item = dist(gen);
        
        used_items[selected_item] = true;
        current_value += items[selected_item].value;
        current_capacity -= items[selected_item].weight;
    }
    
    return current_value;
}


void AntColonyOptimization::update_pheromone_trails(int best_value) {
    for (size_t i = 0; i < pheromone_trails_.size(); ++i) {
        for (size_t j = 0; j < pheromone_trails_[i].size(); ++j) {
            pheromone_trails_[i][j] *= (1 - rho);
        }
    }

    double delta_tau = q / best_value;

    for (size_t i = 0; i < items.size(); ++i) {
        pheromone_trails_[i][1] += delta_tau;
    }
}

double AntColonyOptimization::solve(int iteration, int total_iterations) {
    initialize_pheromone_trails();
    initialize_heuristic_information();

    int best_value = 0;

    for (int iter = 0; iter < num_iterations; ++iter) {
        int iteration_best_value = 0;
        
        for (int k = 0; k < num_ants; ++k) {
            int ant_value = construct_solution();
            iteration_best_value = std::max(iteration_best_value, ant_value);
        }

        best_value = std::max(best_value, iteration_best_value);
        update_pheromone_trails(iteration_best_value);

        double progress = static_cast<double>(iter) / num_iterations;
        print_progress_bar((iteration * num_iterations + iter) / static_cast<double>(total_iterations * num_iterations));
    }

    return best_value;
}
