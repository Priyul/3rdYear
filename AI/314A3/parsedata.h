#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <chrono>

#include <vector>
#include <algorithm>
#include <random>
#include <utility>
#include <numeric>

using namespace std;
class Parser {
public:    
    Parser(const string &filename);
    void inputData();
    void removeIncompleteData();
    void replaceIncompleteDataWithRandomBinary();
    void shuffleData();

    void parseTrainingData();
    void parseTestData();

    pair<vector<vector<double>>, vector<double>> getTrainingData();
    pair<vector<vector<double>>, vector<double>> getTestingData();
private:
    const string& filename;
    vector<string> instances;

    pair<vector<vector<double>>, vector<double>> training_data;
    pair<vector<vector<double>>, vector<double>> testing_data;

    vector<string> split(const string &s, char delimiter);
    vector<double> processInstance(const vector<string>& tokens);
};