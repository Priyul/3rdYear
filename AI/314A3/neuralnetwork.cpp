#include "neuralnetwork.h"
NeuralNetwork::NeuralNetwork(vector<Layer> layers, pair<vector<vector<double>>, vector<double>> trainingdata, double learningRate, int epochs) : learningRate(learningRate), epochs(epochs) {
    for (const Layer &layer : layers) {
        this->layers.push_back(layer);
    }

    this->input = trainingdata.first;
    this->expectedOutput = trainingdata.second;
    bestEpoch = 0;
}


double NeuralNetwork::binaryCrossEntropy(double expected, double output) {
    return -expected * log(output) - (1 - expected) * log(1 - output);
}

double NeuralNetwork::max(double x, double y) {
    if (x > y) {
        return x;
    } else {
        return y;
    }
    return 0;
}

void NeuralNetwork::setLearningRate(double rate) {
    this->learningRate = rate;
}

double NeuralNetwork::ReLU(double x) {
   return max(0,x); 
}

double NeuralNetwork::leakyReLU(double x) {
    double alpha = 0.01;
    return x >= 0 ? x : alpha * x;
}


double NeuralNetwork::sigmoid(double x) {
    double result = 1.0 / (1.0 + exp(-x));
    double min_val = 0.00001;
    double max_val = 0.99999;
    if (result < min_val) {
        return min_val;
    } else if (result > max_val) {
        return max_val;
    } else {
        return result;
    }
}


double NeuralNetwork::train() {
    const int patience = 100;  // number of epochs to wait before stopping if no improvement
    int wait = 0;
    double bestError = std::numeric_limits<double>::max(); // best error so far
    double totalError = 0;
    for (int epoch = 0; epoch < epochs; epoch++) {
        // double totalError = 0;
        totalError = 0;
        for (int i = 0; i < input.size(); i++) {
            feedforward(input[i]);
            backpropagate(expectedOutput[i]);
            for (Neuron& neuron : layers.back().neurons) {
                totalError += binaryCrossEntropy(expectedOutput[i], neuron.output);
            }
        }

        //Early stopping check
        // if (totalError < bestError) {
        //     bestError = totalError;
        //     wait = 0; // reset waiting counter
        // } else {
        //     if (++wait >= patience) {
        //         cout << "Early stopping at epoch " << epoch << ", best error: " << bestError << endl;
        //         return; // stop training
        //     }
        // }
        // this->learningRate *= (1 - this->learningRateDecay);
        if (totalError < bestError) {
            bestError = totalError;
            bestEpoch = epoch;
        }
        cout << "Error at the end of epoch " << epoch << " = " << totalError << endl;
    }
    return totalError;
}


void NeuralNetwork::feedforward(vector<double>& instance) {
    for (int j = 0; j < layers[0].neurons.size(); j++) {
        //for input layer
        layers[0].neurons[j].output = instance[j]; //instance[i][j] // each neuron in the input layer 
                                                    //gets a feature from the attributes
    }

    for (int i = 1; i < layers.size(); i++) {
        for (int j = 0; j < layers[i].neurons.size(); j++) {
            double sum = 0;
            for (int k = 0; k < layers[i-1].neurons.size(); k++) {
                sum += layers[i-1].neurons[k].output * layers[i].neurons[j].weights[k];
            }
            sum+= layers[i].neurons[j].bias;
            // cout << "Sum at layer " << i << " on epoch " << epochs << ": " << sum << endl; 
            if (i != layers.size() - 1) {  // if not output layer
                layers[i].neurons[j].output = leakyReLU(sum);
            } else {
                layers[i].neurons[j].output = sigmoid(sum);
            }
        }
    }
}

void NeuralNetwork::backpropagate(double expectedOutput) {
    Layer& outputLayer = layers.back();
    for (int i = 0; i < outputLayer.neurons.size(); i++) {
        double output = outputLayer.neurons[i].output;
        //output = round(output * 100.0) / 100.0;
        //double sigmoid_derivative = output * (1 - output);
        // outputLayer.neurons[i].error = (output - expectedOutput) / (output * (1 - output));
        outputLayer.neurons[i].error = (output - expectedOutput);
        // cout << "output: " << output << endl;
        // cout << "output layer error" << outputLayer.neurons[i].error << endl;

    }

    for (int i = layers.size()-2; i > 0; i--) { 
        Layer& currentLayer = layers[i];
        Layer& nextLayer = layers[i+1];
        for (int j = 0; j < currentLayer.neurons.size(); j++) {
            double output = currentLayer.neurons[j].output;
            // double relu_derivative = output > 0 ? 1 : 0;
            double relu_derivative = output > 0 ? 1 : 0.01;
            double errorSum = 0;
            for (int k = 0; k < nextLayer.neurons.size(); k++) {
                // cout << "layer no:" << i << endl << "node number:" << k << endl << endl;
                // cout << "actual node error: " << nextLayer.neurons[k].weights[j] * nextLayer.neurons[k].error << endl;
                errorSum += nextLayer.neurons[k].weights[j] * nextLayer.neurons[k].error;
            }
            currentLayer.neurons[j].error = errorSum * relu_derivative;
        }
    }

    for (int i = 1; i < layers.size(); i++) {
        Layer& currentLayer = layers[i];
        Layer& prevLayer = layers[i-1];
        for (int j = 0; j < currentLayer.neurons.size(); j++) {
            for (int k = 0; k < currentLayer.neurons[j].weights.size(); k++) {
                currentLayer.neurons[j].weights[k] -= learningRate * currentLayer.neurons[j].error * prevLayer.neurons[k].output;
            }
            currentLayer.neurons[j].bias -= learningRate * currentLayer.neurons[j].error;
        }
    }
}

double NeuralNetwork::getBestEpoch() {
    return bestEpoch;
}