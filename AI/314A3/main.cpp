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

    pair<vector<vector<double>>, vector<int>> trainingdata;
    trainingdata = parser->process_data(filename);

    vector<Layer> layers;
    layers.push_back(Layer(51,0)); //input layer, 49 nodes with no weights
    layers.push_back(Layer(25,51)); //hidden layer 1, 25 nodes each connecting to all 49 nodes so each have 49 weights
    layers.push_back(Layer(25,25));
    layers.push_back(Layer(1,25)); //output layer, 1 node connecting to all 25 nodes in the hidden layer so 25 weights

    double learningRate = 0.1;
    int epochs = 100;
    NeuralNetwork* neuralNetwork = new NeuralNetwork(layers, trainingdata, learningRate, epochs);
    neuralNetwork->train();


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