#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

#include <vector>
#include <algorithm>
#include <random>


using namespace std;

class Parser {
public:    
    Parser();
    void inputData(const string &filename);
    void removeIncompleteData();
    void shuffleData();
    void processData();
private:
    const string& filename;
    vector<string> split(const string &s, char delimiter);

    pair<vector<vector<double>>, vector<double>> all_data;
    pair<vector<vector<double>>, vector<double>> training_data;
    pair<vector<vector<double>>, vector<double>> testing_data;
};

write me a small program of class Parser with that will parse the data inside of a textfile for me in c++. Here is the class definition:
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
using namespace std;
