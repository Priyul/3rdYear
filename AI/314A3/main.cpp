#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include "parsedata.h"

// #include "layer.h"
#include "neuralnetwork.h"


using namespace std;

int main() {
    string filename = "breast-cancer.data";
    
    Parser* parser = new Parser();

    pair<vector<vector<double>>, vector<double>> trainingdata;
    trainingdata = parser->process_data(filename);

    vector<Layer> layers;
    layers.push_back(Layer(51,0)); //input layer, 51 nodes with no weights
    layers.push_back(Layer(25,51)); //hidden layer 1, 25 nodes each connecting to all 51 nodes so each have 51 weights
    layers.push_back(Layer(25,25));
    // layers.push_back(Layer(25,25));
    layers.push_back(Layer(1,25)); //output layer, 1 node connecting to all 25 nodes in the hidden layer so 25 weights

    double initialLearningRate = 0.01;
    double finalLearningRate = 0.01; // Or whatever lower bound you want
    int epochs = 100000;
    double learningRateDecay = (initialLearningRate - finalLearningRate) / epochs;
    double learningRate = initialLearningRate;
    
    NeuralNetwork* neuralNetwork = new NeuralNetwork(layers, trainingdata, learningRate, epochs);
    neuralNetwork->train();
    // for (int i = 0; i < epochs; i++) {
    //     neuralNetwork->train();
    //     // learningRate -= learningRateDecay;
    //     // neuralNetwork->setLearningRate(learningRate);
    // }


    return 0;
}

    // for (int i = 0; i < layers.size(); i++) {
    //     cout << "Layer " << i << ":" << endl;
    //     for (int j = 0; j < layers[i].neurons.size(); j++) {
    //         cout << "Neuron no." << j << ":" << endl;
    //         for (int k = 0; k < layers[i].neurons[j].weights.size(); k++) {
    //             cout << "Neuron weight." << k << ": " << layers[i].neurons[j].weights[k] << endl;
    //         } 
    //     }
    // } //weights initialization works