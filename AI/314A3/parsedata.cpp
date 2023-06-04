#include "parsedata.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

Parser::Parser(const string &filename) : filename(filename) {}



vector<string> Parser::split(const string &s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void Parser::inputData() {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            instances.push_back(line);
        }
        file.close();
    } else {
        cout << "Unable to open file" << endl;
    }
}

void Parser::removeIncompleteData() {
    for (auto it = instances.begin(); it != instances.end();) {
        if (it->find("?") != string::npos) {
            it = instances.erase(it);
        } else {
            ++it;
        }
    }
}

void Parser::replaceIncompleteDataWithRandomBinary() {
    std::srand(std::time(0));

    for (auto& instance : instances) {
        size_t pos = 0;
        while ((pos = instance.find("?", pos)) != std::string::npos) {
            // Replace "?" with a random 0 or 1
            instance.replace(pos, 1, std::to_string(std::rand() % 2));
        }
    }
}

void Parser::shuffleData() {
    std::vector<int> indices(instances.size());
    for (size_t i = 0; i < indices.size(); ++i) {
        indices[i] = i;
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g);

    std::vector<string> shuffled_instances(instances.size());
    for (size_t i = 0; i < indices.size(); ++i) {
        shuffled_instances[i] = instances[indices[i]];
    }
    instances.swap(shuffled_instances);
}


vector<double> Parser::processInstance(const vector<string>& tokens) {
    vector<double> instance(51, 0);

    bool missing;

    if (tokens[1] == "10-19") 
        instance[0] = 1;
    else if (tokens[1] == "20-29") 
        instance[1] = 1;
    else if (tokens[1] == "30-39") 
        instance[2] = 1;
    else if (tokens[1] == "40-49") 
        instance[3] = 1;
    else if (tokens[1] == "50-59") 
        instance[4] = 1;
    else if (tokens[1] == "60-69") 
        instance[5] = 1;
    else if (tokens[1] == "70-79") 
        instance[6] = 1;
    else if (tokens[1] == "80-89") 
        instance[7] = 1;
    else if (tokens[1] == "90-99") 
        instance[8] = 1;
    else {
        cout << "Missing data for attribute 1" << endl;
        missing = true;
    }

    if (tokens[2] == "lt40") 
        instance[9] = 1;
    else if (tokens[2] == "ge40") 
        instance[10] = 1;
    else if (tokens[2] == "premeno") 
        instance[11] = 1;
    else {
        cout << "Missing data for attribute 2" << endl;
        missing = true;
    }
    

    if (tokens[3] == "0-4") 
        instance[12] = 1;
    else if (tokens[3] == "5-9")
        instance[13] = 1;
    else if (tokens[3] == "10-14")
        instance[14] = 1;
    else if (tokens[3] == "15-19")
        instance[15] = 1;
    else if (tokens[3] == "20-24")
        instance[16] = 1;
    else if (tokens[3] == "25-29")
        instance[17] = 1;
    else if (tokens[3] == "30-34")
        instance[18] = 1;
    else if (tokens[3] == "35-39")
        instance[19] = 1;
    else if (tokens[3] == "40-44")
        instance[20] = 1;
    else if (tokens[3] == "45-49")
        instance[21] = 1;
    else if (tokens[3] == "50-54")
        instance[22] = 1;
    else if (tokens[3] == "55-59")
        instance[23] = 1;    
    else {
        cout << "Missing data for attribute 3" << endl;
        missing = true;
    }

    if (tokens[4] == "0-2") 
        instance[24] = 1;
    else if (tokens[4] == "3-5") 
        instance[25] = 1;
    else if (tokens[4] == "6-8") 
        instance[26] = 1;
    else if (tokens[4] == "9-11") 
        instance[27] = 1;
    else if (tokens[4] == "12-14") 
        instance[28] = 1;
    else if (tokens[4] == "15-17") 
        instance[29] = 1;
    else if (tokens[4] == "18-20") 
        instance[30] = 1;
    else if (tokens[4] == "21-23") 
        instance[31] = 1;
    else if (tokens[4] == "24-26") 
        instance[32] = 1;
    else if (tokens[4] == "27-29") 
        instance[33] = 1;
    else if (tokens[4] == "30-32") 
        instance[34] = 1;
    else if (tokens[4] == "33-35") 
        instance[35] = 1;
    else if (tokens[4] == "36-39") 
        instance[36] = 1;
    else {
        cout << "Missing data for attribute 4" << endl;
        missing = true;
    }

    if (tokens[5] == "yes") 
        instance[37] = 1;
    else if (tokens[5] == "no") 
        instance[38] = 1;
    else {
        cout << "Missing data for attribute 5, data replaced" << endl;
        std::srand(1234);
        int random_binary = std::rand() % 2;
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(37, 38);
        int random_index = distr(gen);

        instances[38] = 1;

        cout << "instance[" << random_index << "] replaced with " << "1" << endl << endl;

        // missing = true;
    }

    if (tokens[6] == "1") 
        instance[39] = 1;
    else if (tokens[6] == "2") 
        instance[40] = 1;
    else if (tokens[6] == "3") 
        instance[41] = 1;
    else {
        cout << "Missing data for attribute 6" << endl;
        // missing = true;
        std::srand(1234);
        int random_binary = std::rand() % 2;
        
        int random_index = std::rand() % 3;
        instance[random_index] = random_binary;

        cout << "instance[" << random_index << "] replaced with " << random_binary << endl << endl;
    }

    if (tokens[7] == "left") 
        instance[42] = 1;
    else if (tokens[7] == "right")
        instance[43] = 1;
    else {
        cout << "Missing data for attribute 7" << endl;
        missing = true;
    }

    // cout << "tokens[8] value: " << tokens[8] << endl;
    if (tokens[8] == "left_up") 
        instance[44] = 1;
    else if (tokens[8] == "left_low") 
        instance[45] = 1;
    else if (tokens[8] == "right_up") 
        instance[46] = 1;
    else if (tokens[8] == "right_low") 
        instance[47] = 1;
    else if (tokens[8] == "central") 
        instance[48] = 1;
    else {
        cout << "Missing data for attribute 8, data replaced" << endl;
        // missing = true;
        std::srand(1234);
        int random_binary = std::rand() % 2;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(44, 48);
        int random_index = distr(gen);
        random_binary = 1;
        instance[46] = 1;
        cout << "instance[" << random_index << "] replaced with " << "1" << endl << endl;
    }


    if (tokens[9] == "yes") {
        instance[49] = 1;
    } else if (tokens[9] == "no") {
        instance[50] = 1;
    } else {
        cout << "Missing data for attribute 8" << endl;
        missing = true;
    }

    return instance;
}

void Parser::parseTrainingData() {
    // shuffleData();

    int training_size = instances.size() * 0.8;

    // cout << "Training data content:" << endl;
    // for (int i = 0; i < training_size; i++) {
    //     cout << instances[i] << endl;
    // } cout << endl << endl << endl << endl;
    
    for (int i = 0; i < training_size; i++) {
        vector<string> tokens = split(instances[i], ',');
        vector<double> instance = processInstance(tokens);

        training_data.first.push_back(instance);
        training_data.second.push_back(tokens[0] == "no-recurrence-events" ? 0 : 1);
    }
}

void Parser::parseTestData() {
    int training_size = instances.size() * 0.8; 

    // cout << "TESTING data content:" << endl;
    // for (int i = training_size; i < instances.size(); i++) {
    //     cout << instances[i] << endl;
    // } cout << endl;

    for (int i = training_size; i < instances.size(); i++) {
        vector<string> tokens = split(instances[i], ',');
        vector<double> instance = processInstance(tokens);
        testing_data.first.push_back(instance);
        testing_data.second.push_back(tokens[0] == "no-recurrence-events" ? 0 : 1);
    }
}
pair<vector<vector<double>>, vector<double>> Parser::getTrainingData() {
    return training_data;
}

pair<vector<vector<double>>, vector<double>> Parser::getTestingData() {
    return testing_data;
}