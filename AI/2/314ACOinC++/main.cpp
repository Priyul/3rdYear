#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <numeric>

using namespace std;

struct Item {
    double weight;
    double value;
};

pair<vector<Item>, double> parse_file(const string& filename) {
    ifstream file(filename);
    double n_items, capacity;
    file >> n_items >> capacity;

    vector<Item> items(n_items);
    for (int i = 0; i < n_items; i++)
        file >> items[i].value >> items[i].weight;

    return make_pair(items, capacity);
}

struct Ant {
    vector<int> items_taken;
    double total_weight = 0;
    double total_value = 0;
};

int select_next_item(vector<double>& pheromones, vector<double>& attractiveness, vector<double>& probabilities, const vector<int>& items_taken, double alpha, double beta) {
    double total = 0;
    for (int i = 0; i < pheromones.size(); i++) {
        if (items_taken[i] == 0) { // consider items that have not been taken
            probabilities[i] = pow(pheromones[i], alpha) * pow(attractiveness[i], beta);
            total += probabilities[i];
        }
    }
    double r = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
    double sum = 0;
    for (int i = 0; i < probabilities.size(); i++) {
        if (items_taken[i] == 0) { //  consider items that have not been taken
            sum += probabilities[i] / total;
            if (r <= sum) {
                return i;
            }
        }
    }
    return -1; //  -1 if all items have been taken
}

void update_pheromone(vector<double>& pheromones, const Ant& ant, double best_value, double rho) {
    for (int i = 0; i < ant.items_taken.size(); i++) {
        if (ant.items_taken[i]) {
            pheromones[i] += 1 / (1 + ((best_value - ant.total_value) / best_value));
        }
        pheromones[i] *= rho;
    }
}

double ant_colony_optimization(vector<Item>& items, double capacity, int n_ants = 5, int n_iterations = 60, double decay = 0.75, double alpha = 0.1, double beta = 2.0, double rho = 0.5) {
    srand(time(0));
    int n_items = items.size();
    vector<double> pheromones(n_items, 1);
    vector<double> attractiveness(n_items, 0);
    vector<double> probabilities(n_items, 0);
    double best_value = 0;

    for (int i = 0; i < n_items; i++) {
        attractiveness[i] = items[i].value / pow(items[i].weight, 2);
    }
    // for (int i = 0; i < n_items; i++) {
    //     attractiveness[i] = items[i].value / items[i].weight;
    // }


    for (int iteration = 0; iteration < n_iterations; iteration++) {
        vector<Ant> ants(n_ants);
        for (Ant& ant : ants) {
        ant.items_taken.resize(n_items, 0);
            for(int i = 0; i < n_items; ++i) {
                int itemIndex = select_next_item(pheromones, attractiveness, probabilities, ant.items_taken, alpha, beta);
                if (itemIndex != -1 && items[itemIndex].weight + ant.total_weight <= capacity) {
                    ant.items_taken[itemIndex] = 1;
                    ant.total_weight += items[itemIndex].weight;
                    ant.total_value += items[itemIndex].value;
                }
            }
            if (ant.total_value > best_value) {
                best_value = ant.total_value;
            }
        }   
        for (const Ant& ant : ants) {
            update_pheromone(pheromones, ant, best_value, rho);
        }
    }
    return best_value;
}

int main() {
    srand(time(0)); 
    vector<string> file_names = {"f1_l-d_kp_10_269.txt", "f2_l-d_kp_20_878.txt", "f3_l-d_kp_4_20.txt", "f4_l-d_kp_4_11.txt", "f5_l-d_kp_15_375.txt", "f6_l-d_kp_10_60.txt", "f7_l-d_kp_7_50.txt", "knapPI_1_100_1000_1.txt", "f8_l-d_kp_23_10000.txt", "f9_l-d_kp_5_80.txt", "f10_l-d_kp_20_879.txt"};
    for (const auto& file_name : file_names) {
        auto start_time = clock();
        auto [items, capacity] = parse_file(file_name);
        double best_value = ant_colony_optimization(items, capacity);

        auto end_time = clock();
        double runtime = double(end_time - start_time) / CLOCKS_PER_SEC;

        cout << "File name: " << file_name << '\n';
        cout << "Best fitness: " << best_value << '\n';
        cout << "Best solution: ";

        cout << "\nRuntime: " << runtime << "s\n-------------\n";
    }
    return 0;
}
