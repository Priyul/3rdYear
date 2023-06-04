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

// #include "layer.h"
#include "neuralnetwork.h"


using namespace std;

void alternateData(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        throw std::runtime_error("Failed to open input file.");
    }

    std::vector<std::string> noRecurrence;
    std::vector<std::string> recurrence;
    std::string line;

    while (std::getline(infile, line)) {
        if (line.substr(0, 20) == "no-recurrence-events") {
            noRecurrence.push_back(line);
        } else {
            recurrence.push_back(line);
        }
    }
    infile.close();  // Close the file after reading

    // Random number generation for random insertion
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<std::string> combinedData;
    int indexNo = 0;
    int indexRe = 0;

    while (indexNo < noRecurrence.size() || indexRe < recurrence.size()) {
        if (indexNo < noRecurrence.size()) {
            combinedData.push_back(noRecurrence[indexNo++]);
        }
        if (indexRe < recurrence.size()) {
            combinedData.push_back(recurrence[indexRe++]);
        }
    }

    // Open the file for writing and check if it was successful
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        throw std::runtime_error("Failed to open output file.");
    }

    // Write the alternated lines back to the file
    for (const auto& line : combinedData) {
        outfile << line << '\n';
    }
    outfile.close();
}

int main() {
    string filename = "breast-cancer.data";
    alternateData(filename);
    Parser* parser = new Parser(filename);
    // parser->shuffleFile();
    parser->inputData();
    // parser->removeIncompleteData();
    // parser->replaceIncompleteDataWithRandomBinary();
    // parser->shuffleData();
    parser->parseTrainingData();
    parser->parseTestData();
    pair<vector<vector<double>>, vector<double>> trainingdata = parser->getTrainingData();
    pair<vector<vector<double>>, vector<double>> testingData = parser->getTestingData();
    

    vector<vector<Layer>> allLayers;
    vector<Layer> layers;

    double lowestError = 99999;
    int hiddenNum = 99999;
    int bestEpoch = 99999;
    for (int hidden = 1; hidden < 51; hidden++) {
        vector<Layer> tempLayer;
        tempLayer.push_back(Layer(51,0, "input"));
        tempLayer.push_back(Layer(hidden, 51, "hidden 1")); //hidden layer 1
        // tempLayer.push_back(Layer(hidden, hidden, "hidden 2")); //hidden layer 2
        // tempLayer.push_back(Layer(hidden, hidden, "hidden 3")); //hidden layer 3
        // tempLayer.push_back(Layer(hidden, hidden)); //hidden layer 4
        // tempLayer.push_back(Layer(hidden, hidden)); //hidden layer 5
        tempLayer.push_back(Layer(1,hidden, "output"));

        allLayers.push_back(tempLayer);
    }

    int curious = 0;
    double bestLearningRate = 99999;
    float bestCorrectCount = 0;
    float bestWrongCount = 99999;
    double currBestEpoch = 99999;

    double learningRateArray[28] = {0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 
    0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1};

//    for (int i = 0; i < 28; i++) {
        // for (int hidden = 0; hidden < 50; hidden++) {
        //     double learningRate = 0.055111;
        //     vector<Layer> currLayer = allLayers.at(hidden);
        //     int epochs = 1050;
            
        //     NeuralNetwork* neuralNetwork = new NeuralNetwork(currLayer, trainingdata, learningRate, epochs);
        //     double error = neuralNetwork->train();
        //     currBestEpoch = neuralNetwork->getBestEpoch();

        //     curious+=neuralNetwork->curious;

        //     if (error < lowestError ) {
        //         lowestError = error;
        //         hiddenNum = hidden;
        //         bestEpoch = currBestEpoch;
        //         bestLearningRate = learningRate;

        //         // neuralNetwork->testNetwork(testingData);
        //         // bestWrongCount = neuralNetwork->wrongCount;
        //         // bestCorrectCount = neuralNetwork->correctCount;
        //     }
        //         // neuralNetwork->testNetwork(testingData);
        //         // if (neuralNetwork->wrongCount < bestWrongCount) {
        //         //     bestWrongCount = neuralNetwork->wrongCount;
        //         //     bestCorrectCount = neuralNetwork->correctCount;
        //         //     lowestError = error;
        //         //     hiddenNum = hidden;
        //         //     bestEpoch = currBestEpoch;
        //         //     bestLearningRate = learningRate;
        //         // }

        //     //cout << "lowest error:" << lowestError << endl << "hidden layers nodes:" << hiddenNum+1 << endl;
        // }
    // }
    // cout << endl;
    // cout << "lowest possible error:" << lowestError << endl;
    // cout << "ideal number of hidden layers nodes:" << hiddenNum << endl;
    // cout << "optimal number of epochs: " << currBestEpoch << endl;
    // cout << "best learning rate: " << bestLearningRate << endl;
    // cout << "Best correct count:" << bestCorrectCount << endl;
    // cout << "Best wrong count:" << bestWrongCount << endl;
    // cout << "accuracy:" << (bestCorrectCount)/(bestCorrectCount+bestWrongCount)*100 << endl;
    /* driver code!!! */
        layers.push_back(Layer(51,0, "input")); //input layer, 51 nodes with no weights
        layers.push_back(Layer(37,51, "hidden 1")); //hidden layer 1, 29 nodes each connecting to all 51 nodes so each have 51 weights
        layers.push_back(Layer(37,37, "hidden 2")); //hidden layer 2
        // layers.push_back(Layer(37,37, "hidden 3")); //hidden layer 3
        layers.push_back(Layer(1,37, "output")); //output layer, 1 node connecting to all 29 nodes in the hidden layer so 29 weights

        double initialLearningRate = 0.06;
        int epochs = 749; //1736

        double learningRate = initialLearningRate;
        
        NeuralNetwork* neuralNetwork = new NeuralNetwork(layers, trainingdata, initialLearningRate, epochs);
        // neuralNetwork->testNetwork(testingData);
        double finalError = neuralNetwork->train();
        cout << "Final error: " << finalError << endl;
        cout << endl;
        // cout << "best epoch:" << neuralNetwork->getBestEpoch() << endl;

        cout << "Testing data feedforward:" << endl;
        neuralNetwork->testNetwork(testingData);
        cout << endl;
        cout << "True positive:" << neuralNetwork->TP << endl;
        cout << "True negative:" << neuralNetwork->TN << endl;
        cout << "False positive:" << neuralNetwork->FP << endl;
        cout << "False negative:" << neuralNetwork->FN << endl;

        double precision = neuralNetwork->TP/(neuralNetwork->TP+neuralNetwork->FP);
        float recall = neuralNetwork->TP/(neuralNetwork->TP+neuralNetwork->FN);
        cout << "precision: " << precision << endl << "recall: " << recall << endl;

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