#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include "parsedata.h"
#include <filesystem>
#include <chrono>
#include <stdexcept>

#include "tree.h"

using namespace std;

int main() {
    string filename = "breast-cancer.data";
    Parser* parser = new Parser(filename);
    parser->inputData();
    parser->parseTrainingData();
    parser->parseTestData();

    pair<vector<vector<double>>, vector<double>> trainingdata = parser->getTrainingData();
    pair<vector<vector<double>>, vector<double>> testingData = parser->getTestingData();

    int max_features = trainingdata.first[0].size();
    int depth = 10;
    int populationSize = 100;
    int generations = 50;

    vector<Tree*> population;
    for (int i = 0; i < populationSize; i++) {
        Tree* tree = new Tree(depth, max_features);
        population.push_back(tree);
    }

    for (int generation = 0; generation < 50; generation++) {
        // Evaluate fitness of individuals in the population
        vector<double> fitnesses;
        for (Tree* tree : population) {
            fitnesses.push_back(tree->evaluate_fitness(trainingdata.first, trainingdata.second));
        }
        // Select fit individuals for reproduction -> tournament selection
        vector<Tree*> selected;
        int tournament_size = 5;
        for (int i = 0; i < populationSize; i++) {
            double max_fitness = -1;
            Tree* selected_tree = nullptr;
            for (int j = 0; j < tournament_size; j++) {
                int index = rand() % populationSize;
                if (fitnesses[index] > max_fitness) {
                    max_fitness = fitnesses[index];
                    selected_tree = population[index];
                }
            }
            cout << "Generation " << generation << ": best fitness = " << max_fitness << endl;
            selected.push_back(selected_tree);
        }
        // Perform crossover and mutation to generate offspring
        // crossoverAndMutate(selected);
        vector<Tree*> offspring;
        for (int i = 0; i < selected.size(); i += 2) {
            Tree* child1 = new Tree(*selected[i]); // Copy parents
            Tree* child2 = new Tree(*selected[i+1]);
            
            // Perform crossover (swap subtrees)
            Node* temp = child1->root->left_child;
            child1->root->left_child = child2->root->left_child;
            child2->root->left_child = temp;

            // Perform mutation (alter a random node)
            if (rand() / (double)RAND_MAX < 0.1) { // Mutation rate = 0.1
                child1->root->left_child->threshold += ((double)rand() / RAND_MAX - 0.5) * 0.1; // Add a small random value to the threshold
                if (child1->root->left_child->threshold < 0.0) child1->root->left_child->threshold = 0.0; // Ensure the threshold is still between 0.0 and 1.0
                if (child1->root->left_child->threshold > 1.0) child1->root->left_child->threshold = 1.0;
            }

            offspring.push_back(child1);
            offspring.push_back(child2);
        }

        // Replace least fit individuals in population with offspring 
        vector<pair<double, Tree*>> fitness_tree_pairs;
        for (int i = 0; i < populationSize; i++) {
            fitness_tree_pairs.push_back(make_pair(fitnesses[i], population[i]));
        }
        sort(fitness_tree_pairs.begin(), fitness_tree_pairs.end());
        for (int i = 0; i < offspring.size(); i++) {
            delete fitness_tree_pairs[i].second; // Delete the old tree
            fitness_tree_pairs[i].second = offspring[i]; // Replace with the new tree
        }
        for (int i = 0; i < populationSize; i++) {
            population[i] = fitness_tree_pairs[i].second; // Update the population
        }
    }



    return 0;
}