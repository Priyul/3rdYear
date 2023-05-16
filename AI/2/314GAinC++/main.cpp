#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

// Step 1: Parse the file and load data
pair<vector<pair<double, double> >, double> parse_file(const string& filename) {
    ifstream file(filename);
    double n_items, capacity;
    file >> n_items >> capacity;

    vector<pair<double, double> > items(n_items);
    for (int i = 0; i < n_items; i++)
        file >> items[i].first >> items[i].second;

    return make_pair(items, capacity);
}

// Step 2: Define the chromosome structure and population
vector<vector<int> > generate_population(int n_items, int pop_size) {
    vector<vector<int> > population(pop_size, vector<int>(n_items));
    for (auto& chromosome : population)
        for (auto& gene : chromosome)
            gene = rand() % 2;
    return population;
}

// Step 3: Define fitness function
double calculate_fitness(const vector<int>& chromosome, const vector<pair<double, double> >& items, double capacity) {
    double total_value = 0;
    double total_weight = 0;
    for (int i = 0; i < items.size(); i++) {
        if (chromosome[i]) {
            total_value += items[i].first;
            total_weight += items[i].second;
        }
    }
    return total_weight <= capacity ? total_value : -1e9;
}

// Step 4: Define selection, crossover, and mutation operations
vector<int> tournament_selection(const vector<vector<int> >& population, const vector<pair<double, double> > &items, double capacity, int tournament_size) {
    vector<vector<int> > tournament;
    for (int i = 0; i < tournament_size; i++)
        tournament.push_back(population[rand() % population.size()]);
    auto best_chromosome = max_element(tournament.begin(), tournament.end(),
        [&](const vector<int>& a, const vector<int>& b) -> bool {
            return calculate_fitness(a, items, capacity) < calculate_fitness(b, items, capacity);
        });
    return *best_chromosome;
}

pair<vector<int>, vector<int> > one_point_crossover(const vector<int>& parent1, const vector<int>& parent2) {
    int crossover_point = rand() % (parent1.size() - 2) + 1;
    vector<int> child1(parent1.begin(), parent1.begin() + crossover_point);
    child1.insert(child1.end(), parent2.begin() + crossover_point, parent2.end());
    vector<int> child2(parent2.begin(), parent2.begin() + crossover_point);
    child2.insert(child2.end(), parent1.begin() + crossover_point, parent1.end());
    return make_pair(child1, child2);
}

void bit_flip_mutation(vector<int>& chromosome, double mutation_rate) {
    for (auto& gene : chromosome)
        if ((double) rand() / RAND_MAX < mutation_rate)
            gene = 1 - gene;
}

// Step 5: Implement the main genetic algorithm loop
pair<vector<int>, double> genetic_algorithm(const vector<pair<double, double> >& items, double capacity, int pop_size = 100, int generations = 1000,
    int tournament_size = 5, double mutation_rate = 0.01) {
    int n_items = items.size();
    auto population = generate_population(n_items, pop_size);
    for (int g = 0; g < generations; g++) {
        vector<vector<int> > new_population;
        for (int i = 0; i < pop_size / 2; i++) {
            auto parent1 = tournament_selection(population, items, capacity, tournament_size);
            auto parent2 = tournament_selection(population, items, capacity, tournament_size);
            auto parents = one_point_crossover(parent1, parent2);
            vector<int> child1 = parents.first;
            vector<int> child2 = parents.second;
            bit_flip_mutation(child1, mutation_rate);
            bit_flip_mutation(child2, mutation_rate);
            new_population.push_back(child1);
            new_population.push_back(child2);
        }
        population = move(new_population);
    }

    auto best_chromosome = max_element(population.begin(), population.end(),
        [&](const vector<int>& a, const vector<int>& b) {
            return calculate_fitness(a, items, capacity) < calculate_fitness(b, items, capacity);
        });
    double best_fitness = calculate_fitness(*best_chromosome, items, capacity);
    return make_pair(*best_chromosome, best_fitness);
}

int main() {
    srand(time(0)); // Initialize random seed
    vector<string> file_names = {"f1_l-d_kp_10_269.txt", "f2_l-d_kp_20_878.txt", "f3_l-d_kp_4_20.txt", "f4_l-d_kp_4_11.txt", "f5_l-d_kp_15_375.txt", "f6_l-d_kp_10_60.txt", "f7_l-d_kp_7_50.txt", "knapPI_1_100_1000_1.txt", "f8_l-d_kp_23_10000.txt", "f9_l-d_kp_5_80.txt", "f10_l-d_kp_20_879.txt"};

    for (const auto& file_name : file_names) {
        auto start_time = clock();

        auto file_data = parse_file(file_name);
        vector<pair<double, double>> items = file_data.first;
        double capacity = file_data.second;
        const auto [best_chromosome, best_fitness] = genetic_algorithm(items, capacity);


        auto end_time = clock();
        double runtime = double(end_time - start_time) / CLOCKS_PER_SEC;

        cout << "File name: " << file_name << '\n';
        cout << "Best fitness: " << best_fitness << '\n';
        cout << "Best solution: ";
        for (int gene : best_chromosome) cout << gene;
        cout << "\nRuntime: " << runtime << "s\n-------------\n";
    }

    return 0;
}





