#include "parsedata.h"

Parser:: Parser() {

}

vector<string> Parser::split(const string &s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }

    // for (int i = 0; i < tokens.size(); i++) {
    //     cout << tokens[i] << endl;
    // }
    // cout << endl; // data read correctly
    // cout << "Number of data: " << counter << endl;

    return tokens;
}

pair<vector<vector<double>>, vector<double>> Parser::process_testing_data(const string &filename) {
    ifstream file(filename);
    vector<vector<double>> instances;
    vector<double> labels;
    int i = 0;
    int count = 0;

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (i <= 221) {
                i++;
            } else {
                vector<string> tokens = split(line, ',');
                vector<double> instance(51, 0);
                bool missing = false;

                if (tokens[0] == "no-recurrence-events")
                    labels.push_back(0);
                else if (tokens[0] == "recurrence-events")
                    labels.push_back(1);
                else {
                    cout << "Missing data for attribute 0" << endl;
                    missing = true;
                }

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
                    cout << "Missing data for attribute 5" << endl;
                    missing = true;
                }

                if (tokens[6] == "1") 
                    instance[39] = 1;
                else if (tokens[6] == "2") 
                    instance[40] = 1;
                else if (tokens[6] == "3") 
                    instance[41] = 1;
                else {
                    cout << "Missing data for attribute 6" << endl;
                    missing = true;
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
                    cout << "Missing data for attribute 8" << endl;
                    missing = true;
                }

            
                if (tokens[9] == "yes") {
                    instance[49] = 1;
                } else if (tokens[9] == "no") {
                    instance[50] = 1;
                } else {
                    cout << "Missing data for attribute 8" << endl;
                    missing = true;
                }

                if (!missing) {
                    instances.push_back(instance);
                    i++;
                    counter++;
                    string istring = "";
                    for (int i = 0; i < instance.size(); i++) {
                        if (instance[i] == 0) {
                            istring += "0";
                        } else {
                            istring+="1";
                        }
                    }
                    cout << "Instance " << i << ": " << istring << endl;

                } else {
                    missing = false;
                    cout << "missing data, instance " << i << " removed." << endl;
                }
            }
        }
        file.close();
    }
    cout << "Number of data: " << counter << endl;
    cout << "End of testing data parse" << endl;
    return make_pair(instances, labels);
}

pair<vector<vector<double>>, vector<double>> Parser::process_training_data(const string &filename) {
    ifstream file(filename);
    vector<vector<double>> instances;
    vector<double> labels;
    int i = 0;

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            vector<string> tokens = split(line, ',');
            vector<double> instance(51, 0);
            bool missing = false;

                if (tokens[0] == "no-recurrence-events")
                    labels.push_back(0);
                else if (tokens[0] == "recurrence-events")
                    labels.push_back(1);
                else {
                    cout << "Missing data for attribute 0" << endl;
                    missing = true;
                }

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
                    cout << "Missing data for attribute 5" << endl;
                    missing = true;
                }

                if (tokens[6] == "1") 
                    instance[39] = 1;
                else if (tokens[6] == "2") 
                    instance[40] = 1;
                else if (tokens[6] == "3") 
                    instance[41] = 1;
                else {
                    cout << "Missing data for attribute 6" << endl;
                    missing = true;
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
                    cout << "Missing data for attribute 8" << endl;
                    missing = true;
                }

            
                if (tokens[9] == "yes") {
                    instance[49] = 1;
                } else if (tokens[9] == "no") {
                    instance[50] = 1;
                } else {
                    cout << "Missing data for attribute 8" << endl;
                    missing = true;
                }

            if (!missing) {
                instances.push_back(instance);
                counter++;
                i++;
                string istring = "";
                for (int i = 0; i < instance.size(); i++) {
                    if (instance[i] == 0) {
                        istring += "0";
                    } else {
                        istring+="1";
                    }
                }
                cout << "Instance " << i << ": " << istring << endl;
                if (i == (221)) {
                    file.close();
                    cout << "Number of data: " << counter << endl;
                    cout << "End of training data parse" << endl;
                    cout << endl;
                    return make_pair(instances, labels);
                }
            } else {
                missing = false;
                cout << "missing data, instance " << i << " removed." << endl;
            }
        }
        file.close();
    }
    cout << "Number of data: " << counter << endl;
    cout << "End of training data parse" << endl;
    return make_pair(instances, labels);
}