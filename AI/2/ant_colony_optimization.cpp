#include <vector>
#include <cmath>
#include <limits>
#include <random>
#include <string>
#include <iostream>
#include "ant_colony_optimization.h"
#include "bin.h"

AntColonyOptimization::AntColonyOptimization(const std::vector<Bin> &items, int capacity, int num_ants, int num_iterations, double alpha, double beta, double rho, double q)
    : items(items), capacity(capacity), num_ants(num_ants), num_iterations(num_iterations), alpha(alpha), beta(beta), rho(rho), q(q) {
    pheromone.resize(items.size(), std::vector<double>(2, 1.0));
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
int AntColonyOptimization::solve(int iteration, int total_iterations) {
    int best_value = 0;
    std::vector<int> best_solution(items.size(), 0);
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    for (int iter = 0; iter < num_iterations; ++iter) {
        for (int ant = 0; ant < num_ants; ++ant) {
            int current_weight = 0;
            int current_value = 0;
            std::vector<int> solution(items.size(), 0);

            for (int i = 0; i < items.size(); ++i) {
                double p = distribution(generator);
                double probability_sum = 0.0;
                for (int j = 0; j < 2; ++j) {
                    probability_sum += pow(pheromone[i][j], alpha) * pow((1.0 - static_cast<double>(j)), beta);
                }
                double probability = (pow(pheromone[i][1], alpha) * pow((1.0 - static_cast<double>(1)), beta)) / probability_sum;

                if (p < probability && current_weight + items[i].weight <= capacity) {
                    solution[i] = 1;
                    current_weight += items[i].weight;
                    current_value += items[i].value;
                }
            }

            if (current_value > best_value) {
                best_value = current_value;
                best_solution = solution;
            }

            for (int i = 0; i < items.size(); ++i) {
                if (solution[i] == 1) {
                    pheromone[i][1] += q / current_value;
                } else {
                    pheromone[i][0] += q / (capacity - current_weight);
                }
            }
        }

        for (int i = 0; i < items.size(); ++i) {
            for (int j = 0; j < 2; ++j) {
                pheromone[i][j] *= (1.0 - rho);
            }
        }
    }

    return best_value;
}
