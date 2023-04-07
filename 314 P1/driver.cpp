#include <iostream> //input and output 
#include <fstream> //file stream
#include <vector> //vector data structure
#include <string> 
#include <algorithm>
#include <map> //map data structure
#include <ctime> // time elapsed
#include <cstdlib>
#include <sstream> // for reading from the excel file
#include <chrono> // for time elapsed
#include <thread>
#include <mutex>
#include <random> //for random selection
#include <bitset> // main functionality
#include <deque> // for tabu search
#include <queue> //for code complexity reduction 

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif

using namespace std;
std::map<std::string, int> known_optimal_solutions;
bool use_best_first_search = false;
bool use_hill_climbing = false;
bool use_simulated_annealing = false;
bool use_greedy_algorithm = false;

bool use_first_fit = false;
bool use_next_fit = false;
bool use_best_fit = false;

//good for 1D bin packing problem apparently
bool use_first_fit_decreasing = false;
bool use_best_fit_decreasing = false;
bool use_next_fit_decreasing = false;
bool use_worst_fit_decreasing = false;


void print_progress_bar(double progress) {
    int bar_width = 50;

    std::cout << "[";
    int position = static_cast<int>(bar_width * progress);
    for (int i = 0; i < bar_width; ++i) {
        if (i < position)
            std::cout << "=";
        else if (i == position)
            std::cout << ">";
        else
            std::cout << " ";
    }
    std::cout << "] " << static_cast<int>(progress * 100.0) << " %\r";
    std::cout.flush();
}



void read_known_optimal_solutions(const std::string &csv_file) {
    known_optimal_solutions.clear();
    std::ifstream input_file(csv_file);
    //cout << "in read_known_optinmal_soln... outputting csv file name passed in: " << csv_file << endl;

    if (input_file.is_open()) {
        std::string line;


        // Ignore the first 4 lines (header)
        std::getline(input_file, line);
        std::getline(input_file, line);
        std::getline(input_file, line);
        std::getline(input_file, line);

        while (std::getline(input_file, line)) {
            std::istringstream line_stream(line);
            std::string file_name;
            int known_optimal_value;

            std::getline(line_stream, file_name, ';');  // Read the file name (delimiter is a comma)
            line_stream >> known_optimal_value;         // Read the known optimal value

            // Remove the file extension from the file name
            size_t last_dot = file_name.find_last_of('.');
            if (last_dot != std::string::npos) {
                file_name = file_name.substr(0, last_dot);
            }

            known_optimal_solutions[file_name] = known_optimal_value;
            //cout << known_optimal_solutions[file_name] << endl;
            //cout << file_name << "...." << endl;
        }

        input_file.close();
    } else {
        std::cerr << "Unable to open file: " << csv_file << std::endl;
    }
} /* data from excel sheet for comparison to results rendered. */

std::vector<std::string> get_text_files(const std::string &folder_name) {
    std::vector<std::string> text_files;
    text_files.clear(); //impportant so program does not crash 

    #ifdef _WIN32
        WIN32_FIND_DATA find_data;
        HANDLE h_find;
        std::string search_path = folder_name + "\\*.txt";

        h_find = FindFirstFile(search_path.c_str(), &find_data);
        if (h_find != INVALID_HANDLE_VALUE) {
            do {
                text_files.push_back(find_data.cFileName);
            } while (FindNextFile(h_find, &find_data) != 0);
            FindClose(h_find);
        }
    #else
        DIR *dir;
        dirent *entry;
        dir = opendir(folder_name.c_str());
        if (dir) {
            while ((entry = readdir(dir)) != nullptr) {
                std::string file_name = entry->d_name;
                size_t pos = file_name.rfind(".txt");
                if (pos != std::string::npos && pos + 4 == file_name.length()) {
                    text_files.push_back(file_name);
                }
            }
            closedir(dir);
        }
    #endif


    return text_files;
}

struct Data {
    int num_items;
    int bin_capacity;
    std::vector<int> item_sizes;
    string fileName;
};
int first_fit(const Data& data);

Data read_file_data(const std::string &file_path) {
    std::ifstream input_file(file_path);
    Data data;

    if (input_file.is_open()) {
        input_file >> data.num_items;
        input_file >> data.bin_capacity;

        for (int i = 0; i < data.num_items; ++i) {
            int size;
            input_file >> size;
            data.item_sizes.push_back(size);
        }

        input_file.close();
    } else {
        std::cerr << "Unable to open file: " << file_path << std::endl;
    }

    return data;
}

class Bin {
public:
    Bin(int capacity) : capacity(capacity), remaining_capacity(capacity) {}

    bool can_fit(int item_size) const {
        return remaining_capacity >= item_size;
    }

    void add_item(int item_size) {
        remaining_capacity -= item_size;
        items.push_back(item_size);
    }

    void remove_item(int index) {
        remaining_capacity += items[index];
        items.erase(items.begin() + index);
    }

    const std::vector<int>& get_items() const {
        return items;
    }

    bool operator<(const Bin& other) const {
        return remaining_capacity > other.remaining_capacity;
    }

    int capacity;
    int remaining_capacity;
    std::vector<int> items;
};
/* HEURISTICS ::::: */

std::vector<int> best_first_search(const Data& data) {
    std::vector<int> sorted_items = data.item_sizes;
    std::sort(sorted_items.begin(), sorted_items.end(), std::greater<int>());
    return sorted_items;
} 

std::vector<int> hill_climbing(const Data& data) {
    std::vector<int> sorted_items = data.item_sizes;
    std::sort(sorted_items.begin(), sorted_items.end());
    return sorted_items;
} 

std::vector<int> simulated_annealing( Data data, double start_temp, double end_temp, double cooling_rate) {
    std::vector<int> items = data.item_sizes;
    std::random_device rd;
    std::mt19937 g(rd());
    double temperature = start_temp;
    int max_items = 1000;
    
    if (items.size() > max_items) {
        items.resize(max_items);
    }

    while (temperature > end_temp) {
        int index1 = g() % items.size();
        int index2 = g() % items.size();
        std::swap(items[index1], items[index2]);

        Data new_data = data;
        new_data.item_sizes = items;
        int new_bins = first_fit(new_data);
        int delta = new_bins - data.num_items;
        if (delta < 0 || std::exp(-delta / temperature) > (double)g() / g.max()) {
            // cout << "data.num_items = " << data.num_items << endl; 
            // cout << "new_bins = " << new_bins << endl;
            data.num_items = new_bins;
            //cout << "hellooo 1234556" << endl;
        } else {
            std::swap(items[index1], items[index2]); // Undo the swap
        }
        temperature *= cooling_rate;
    }

    return items;
}

std::vector<int> greedy_algorithm(const Data &data) {
    std::vector<int> items = data.item_sizes;
    std::sort(items.begin(), items.end(), std::greater<int>());
    return items;
}

int next_fit(const Data& data) {
    std::vector<Bin> bins;
    for (int item_size : data.item_sizes) {
        if (bins.empty() || !bins.back().can_fit(item_size)) {
            Bin new_bin(data.bin_capacity);
            bins.push_back(new_bin);
        }
        bins.back().add_item(item_size);
    }
    return static_cast<int>(bins.size());
}

int best_fit(const Data& data) {
    std::vector<Bin> bins;
    for (int item_size : data.item_sizes) {
        int best_bin_index = -1;
        int min_remaining_capacity = data.bin_capacity + 1;

        for (int i = 0; i < bins.size(); ++i) {
            if (bins[i].can_fit(item_size) && bins[i].remaining_capacity < min_remaining_capacity) {
                best_bin_index = i;
                min_remaining_capacity = bins[i].remaining_capacity;
            }
        }

        if (best_bin_index == -1) {
            Bin new_bin(data.bin_capacity);
            new_bin.add_item(item_size);
            bins.push_back(new_bin);
        } else {
            bins[best_bin_index].add_item(item_size);
        }
    }
    return static_cast<int>(bins.size());
}

int first_fit_decreasing(const Data& data) {
    Data sorted_data = data;
    std::sort(sorted_data.item_sizes.rbegin(), sorted_data.item_sizes.rend());
    return first_fit(sorted_data);
}

int best_fit_decreasing(const Data& data) {
    Data sorted_data = data;
    std::sort(sorted_data.item_sizes.rbegin(), sorted_data.item_sizes.rend());
    return best_fit(sorted_data);
}

int next_fit_decreasing(const Data& data) {
    std::vector<int> sorted_items = data.item_sizes;
    std::sort(sorted_items.begin(), sorted_items.end(), std::greater<int>());

    std::vector<Bin> bins;
    Bin current_bin(data.bin_capacity);
    for (int item_size : sorted_items) {
        if (current_bin.can_fit(item_size)) {
            current_bin.add_item(item_size);
        } else {
            bins.push_back(current_bin);
            current_bin = Bin(data.bin_capacity);
            current_bin.add_item(item_size);
        }
    }
    bins.push_back(current_bin);
    return static_cast<int>(bins.size());
}

int worst_fit_decreasing(const Data& data) {
    std::vector<int> sorted_items = data.item_sizes;
    std::sort(sorted_items.begin(), sorted_items.end(), std::greater<int>());

    std::vector<Bin> bins;
    for (int item_size : sorted_items) {
        int worst_fit_index = -1;
        int max_remaining_capacity = -1;

        for (int i = 0; i < bins.size(); ++i) {
            if (bins[i].can_fit(item_size) && bins[i].remaining_capacity > max_remaining_capacity) {
                worst_fit_index = i;
                max_remaining_capacity = bins[i].remaining_capacity;
            }
        }

        if (worst_fit_index == -1) {
            Bin new_bin(data.bin_capacity);
            new_bin.add_item(item_size);
            bins.push_back(new_bin);
        } else {
            bins[worst_fit_index].add_item(item_size);
        }
    }
    return static_cast<int>(bins.size());
}


string generateHeuristicList() {
    string result = "No heuristics used";
    if (use_best_first_search) {
        result = "Best first search";
    }
    if (use_hill_climbing) {
        if (result == "No heuristics used") {
            result = "Hill climbing";
        } else {
            result += " and hill climbing";
        }
    }
    if (use_simulated_annealing) {
        if (result == "No heuristics used") {
            result = "Simulated annealing";
        } else {
            result += " and simulated annealing";
        }
    }
    if (use_greedy_algorithm) {
        if (result == "No heuristics used") {
            result = "Greedy algorithm";
        } else {
            result += " and greedy algorithm";
        }
    }
    if (use_next_fit) {
        if (result == "No heuristics used") {
            result = "Next fit";
        } else {
            result += " and next fit";
        }
    } 
    if (use_best_fit) {
        if (result == "No heuristics used") {
            result = "Best fit";
        } else {
            result += " and best fit";
        }
    }
    if (use_first_fit_decreasing) {
        if (result == "No heuristics used") {
            result = "First fit decreasing";
        } else {
            result += " and first fit decreasing";
        }
    }
    if (use_best_fit_decreasing) {
        if (result == "No heuristics used") {
            result = "Best fit decreasing";
        } else {
            result += " and best fit decreasing";
        }
    }
    if (use_first_fit) {
        if (result == "No heuristics used") {
            result = "First fit";
        } else {
            result += " and first fit";
        }
    }
    if (use_next_fit_decreasing) {
        if (result == "No heuristics used") {
            result = "Next fit decreasing";
        } else {
            result += " and Next fit decreasing";
        }
    }
    if (use_worst_fit_decreasing) {
        if (result == "No heuristics used") {
            result = "Worst fit decreasing";
        } else {
            result += " and Worst fit decreasing";
        }
    }
    
    return result;
}


/* END OF HEURISTICS ::::: */

/* BASE ILS ALGORITHM::::: */

int first_fit(const Data& data) {
    std::vector<Bin> bins;
    for (int item_size : data.item_sizes) {
        bool found_bin = false;
        for (Bin& bin : bins) {
            if (bin.can_fit(item_size)) {
                bin.add_item(item_size);
                found_bin = true;
                break;
            }
        }
        if (!found_bin) {
            Bin new_bin(data.bin_capacity);
            new_bin.add_item(item_size);
            bins.push_back(new_bin);
        }
    }
    return static_cast<int>(bins.size());
}

void perturbation(std::vector<int>& items, int num_perturbations) {
    std::random_device rd;
    std::mt19937 g(rd());
    for (int i = 0; i < num_perturbations; ++i) {
        int index1 = g() % items.size();
        int index2 = g() % items.size();
        std::swap(items[index1], items[index2]);
    }
}

int local_search(const Data& data, std::vector<int>& items) {
    int min_bins;
    if (use_first_fit) 
        min_bins = first_fit(data);
    if (use_next_fit) 
        min_bins = next_fit(data);
    if (use_best_fit) 
        min_bins = best_fit(data);
    if (use_first_fit_decreasing) 
        min_bins = first_fit_decreasing(data);
    if (use_best_fit_decreasing) 
        min_bins = best_fit_decreasing(data);
    if (use_next_fit_decreasing) 
        min_bins = next_fit_decreasing(data);
    if (use_worst_fit_decreasing) 
        min_bins = worst_fit_decreasing(data);


    std::priority_queue<Bin> bins;
    for (int item_size : items) {
        if (!bins.empty() && bins.top().can_fit(item_size)) {
            Bin top_bin = bins.top();
            bins.pop();
            top_bin.add_item(item_size);
            bins.push(top_bin);
        } else {
            Bin new_bin(data.bin_capacity);
            new_bin.add_item(item_size);
            bins.push(new_bin);
        }
    }

    int num_bins = static_cast<int>(bins.size());
    //cout << min_bins << ": min bins; num bins: " << num_bins << endl;
    if (num_bins > min_bins) {
        //min_bins = num_bins;
        //cout << "Hiiiiii";
        return min_bins;
    }
    //cout << min_bins << ": min bins; num bins: " << num_bins << endl;
    return min_bins;
}

int iterated_local_search( Data& data, int num_iterations, int num_perturbations) {
    std::vector<int> items = data.item_sizes;

    if (use_best_first_search) {
        items = best_first_search(data);
    }
    
    if (use_hill_climbing) {
        items = hill_climbing(data);
    }
    if (use_simulated_annealing) {
        items = simulated_annealing(data, 1000, 1, 0.99);
    }
    if (use_greedy_algorithm) {
        items = greedy_algorithm(data);
    }

    for (int i = 0; i < num_iterations; ++i) {
        perturbation(items, num_perturbations);
        int new_bins = local_search(data, items);
        if (new_bins < data.num_items) {
            data.num_items = new_bins;
        }
        double progress = static_cast<double>(i + 1) / num_iterations;
        print_progress_bar(progress);
    }
    return data.num_items;
}


/* END OF BASE ILS ALGORITHM ::: */

/* BASE TABU SEARCH ALGORITHM ::: */

// Calculate the delta between two solutions
int delta(const std::vector<int>& a, const std::vector<int>& b) {
    int count = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) {
            count++;
        }
    }
    return count;
}

int evaluate_solution(const Data& data, const std::vector<int>& item_order) {
    std::vector<Bin> bins;

    for (int item_size : item_order) {
        bool found_bin = false;
        for (Bin& bin : bins) {
            if (bin.can_fit(item_size)) {
                bin.add_item(item_size);
                found_bin = true;
                break;
            }
        }
        if (!found_bin) {
            Bin new_bin(data.bin_capacity);
            new_bin.add_item(item_size);
            bins.push_back(new_bin);
        }
    }
    return static_cast<int>(bins.size());
}

std::vector<int> neighborhood_search_tabu(const Data& data, const std::vector<int>& items, const std::deque<std::vector<int> >& tabu_list, int k, int best_result) {
    std::vector<int> candidate_items;
    int min_bins = INT_MAX;
    std::random_device rd;
    std::mt19937 g(rd());

    for (int i = 0; i < k; ++i) {
        std::vector<int> temp_items = items;
        int num_swaps = g() % (items.size() / 2) + 1;

        for (int j = 0; j < num_swaps; ++j) {
            int index1 = g() % items.size();
            int index2 = g() % items.size();
            while (index1 == index2) {
                index2 = g() % items.size();
            }
            std::swap(temp_items[index1], temp_items[index2]);
        }

        int current_bins = evaluate_solution(data, temp_items);
        bool is_tabu = std::find(tabu_list.begin(), tabu_list.end(), temp_items) != tabu_list.end();
        bool aspiration_criterion = current_bins < best_result;

        if (!is_tabu || aspiration_criterion) {
            if (current_bins < min_bins) {
                min_bins = current_bins;
                candidate_items = temp_items;
            }
        }
    }

    return candidate_items;
}


int tabu_search(Data& data, int num_iterations, int tabu_list_size, int k) {
    std::vector<int> items = data.item_sizes;

    items = greedy_algorithm(data);

    int best_result = evaluate_solution(data, items);
    std::vector<int> best_items = items;

    std::deque<std::vector<int> > tabu_list;
    tabu_list.push_back(items);

    for (int i = 0; i < num_iterations; ++i) {
        std::vector<int> candidate_items = neighborhood_search_tabu(data, items, tabu_list, k, best_result);


        int new_result = evaluate_solution(data, candidate_items);

        if (new_result < best_result) {
            best_result = new_result;
            best_items = candidate_items;
        }

        items = candidate_items;
        if (tabu_list.size() >= tabu_list_size) {
            tabu_list.pop_front();
        }
        tabu_list.push_back(items);

        double progress = static_cast<double>(i + 1) / num_iterations;
        print_progress_bar(progress);
    }

    data.item_sizes = best_items;
    return best_result;
}

/* END OF BASE TABU SEARCH ALGO :::: */

/* MAIN FUNCTION CLARITY */
void ILS() {

}

void Tabu() {
    
}

int main() {
    //std::vector<std::string> folder_names = {"Waescher", "Hard28", "Scholl/Scholl_1", "Scholl/Scholl_2", "Scholl/Scholl_3", "Schwerin/Schwerin_1", "Schwerin/Schwerin_2", "Falkenauer/Falkenauer_T", "Falkenauer/Falkenauer_U"};
    std::vector<std::string> folder_names;
    double totalRunTimeILS = 0;
    double totalRunTimeTabu = 0;

    folder_names.push_back("Falkenauer/Falkenauer_T");
    folder_names.push_back("Falkenauer/Falkenauer_U");
    folder_names.push_back("Scholl/Scholl_1");
    folder_names.push_back("Scholl/Scholl_2");
    folder_names.push_back("Scholl/Scholl_3");
    folder_names.push_back("Schwerin/Schwerin_1");
    folder_names.push_back("Schwerin/Schwerin_2");
    folder_names.push_back("Hard28");
    folder_names.push_back("Waescher");
    
    //std::vector<std::string> csv_files = {"CSV/Waescher.csv", "CSV/Hard28.csv", "CSV/Scholl_1.csv", "CSV/Scholl_2.csv", "CSV/Scholl_3.csv", "CSV/Schwerin_1.csv", "CSV/Schwerin_2.csv", "CSV/Falkenauer_T.csv", "CSV/Falkenauer_U.csv"};
    std::vector<std::string> csv_files;

    csv_files.push_back("CSV/Falkenauer_T.csv");
    csv_files.push_back("CSV/Falkenauer_U.csv");
    csv_files.push_back("CSV/Scholl_1.csv");
    csv_files.push_back("CSV/Scholl_2.csv");
    csv_files.push_back("CSV/Scholl_3.csv");
    csv_files.push_back("CSV/Schwerin_1.csv");
    csv_files.push_back("CSV/Schwerin_2.csv");
    csv_files.push_back("CSV/Hard28.csv");
    csv_files.push_back("CSV/Waescher.csv");


    use_best_first_search = true;
    use_hill_climbing = false;
    use_simulated_annealing = false;
    use_greedy_algorithm = false;


    use_first_fit = false;
    use_next_fit = false;
    use_best_fit = true;

    //good for 1D bin packing algorithm apparently
    use_first_fit_decreasing = false;
    use_best_fit_decreasing = true;   
    use_next_fit_decreasing = false;
    use_worst_fit_decreasing = false; 
    string heuristicList = generateHeuristicList();
    // string parameterList = genera
    int arrResults[10][5];
    std::vector<std::string> strResults;
    strResults.push_back("Falkenauer_T");
    strResults.push_back("Falkenauer_U");
    strResults.push_back("Scholl_1");
    strResults.push_back("Scholl_2");
    strResults.push_back("Scholl_3");
    strResults.push_back("Schwerin_1");
    strResults.push_back("Schwerin_2");
    strResults.push_back("Hard28");
    strResults.push_back("Waescher");

    //string strResults[9] = {"Waescher", "Hard28", "Scholl_1", "Scholl_2", "Scholl_3", "Schwerin_1", "Schwerin_2", "Falkenauer_T", "Falkenauer_U"};
    int arrRowCount = 0; //increases as folders change.....
    double arrTime[10][2];
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 12; j++) {
            arrTime[i][j] = 0;
        }
    }
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 5; j++) {
            arrResults[i][j] = 0;
        }
    }

    int count = 0;
    string optStatus = "";
    int optimal_count = 0;
    int near_optimal_count = 0;
    double totTimePerFolder = 0;

    string strResult = "";
    int sum = 0;
    int colCount = 0;

    std::ofstream final_results_ILS("ILSResults.txt");
    std::ofstream final_results_Tabu("TabuResults.txt");
    std::ofstream final_results_table_1("Results1.txt");
    std::ofstream final_results_table_2("Results2.txt");

    //compute totals in 2d array -> add rows 
    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < 9; i++) {
            //goes from row 0 to 8, sum goes in row 9
            sum += arrTime[i][j];
        }
        arrTime[9][j] = sum;
        sum = 0;
    }
    sum = 0;

    /* ILS SEARCH */
    {
        for (int folder_index = 0; folder_index < folder_names.size(); ++folder_index) {
            std::string folder_name = folder_names[folder_index];
            std::string csv_file = csv_files[folder_index];
            read_known_optimal_solutions(csv_file);
            // cout << entry.first.known_optimal_solutions["Falkenauer_t249_07"];
            // Open the results file for writing
            //std::ofstream results_file(folder_name + "_ILS" + "_Results.txt");

            for (const auto &entry : known_optimal_solutions) {
                std::cout << "File: " << entry.first << ", Known optimal value: " << entry.second << std::endl;
            } std::cout << "Known optimal solutions read. Running the algorithm now... " << std::endl << std::endl;
            count = 0;

            cout << "Heuristics used: " << heuristicList << endl;
            //results_file << "Algorithm used: Iterated Local Search" << endl;
            //results_file << "Heuristics used: " << heuristicList << endl << endl;

            std::vector<std::string> text_files = get_text_files(folder_name);

            for (int file_index = 0; file_index < text_files.size(); ++file_index) {
                count++;
                cout << "iteration #" << count << " out of " << text_files.size() << endl;
                std::string file_path = folder_name + "/" + text_files[file_index];
                Data data = read_file_data(file_path);

                auto start_time = std::chrono::high_resolution_clock::now();
                int num_iterations = 1000;  // Adjust as needed
                int num_perturbations = 50;  // Adjust as needed
                int result = iterated_local_search(data, num_iterations, num_perturbations);
                auto end_time = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> runtime = end_time - start_time;

                std::string optimal_value_color;
                optStatus = "";
                if (result == known_optimal_solutions[text_files[file_index].substr(0, text_files[file_index].size() - 4)]) {
                    optimal_value_color = "\033[32m";  // Green
                    optStatus += "Optimal solution found :)";
                    optimal_count++;
                } else if (result == known_optimal_solutions[text_files[file_index].substr(0, text_files[file_index].size() - 4)]+1) {
                    optimal_value_color = "\033[33m";  // Yellow
                    optStatus += "Near-optimal solution found :|";
                    near_optimal_count++;
                } else {
                    optimal_value_color = "\033[31m";  // Red
                    optStatus += "Bad solution found :(";
                }

                //results_file << "File: " << text_files[file_index].substr(0, text_files[file_index].size() - 4) << std::endl;
                //results_file << "Runtime: " << runtime.count() << " ms" << std::endl;
                //results_file << "Calculated Optimal: " << optStatus << " - " << result << std::endl;
                //results_file << "Known Optimal: " << known_optimal_solutions[text_files[file_index].substr(0, text_files[file_index].size() - 4)] << std::endl << std::endl;


                std::cout << "File: " << text_files[file_index] << endl;
                cout<< "Runtime: \033[34m" << runtime.count() << "s\033[0m " << endl;
                totTimePerFolder += runtime.count();
                cout << optimal_value_color << optStatus << "\033[0m " << endl;
                cout << "Calculated Optimal: " << optimal_value_color << result << "\033[0m " << endl;
                //cout << text_files[file_index].substr(0, text_files[file_index].size() - 4) << endl;
                //cout << "hellooooooooo" << endl << known_optimal_solutions["Falkenauer_t120_09"] << endl;
                cout << "Known Optimal: " << known_optimal_solutions[text_files[file_index].substr(0, text_files[file_index].size() - 4)] << std::endl << endl;

                //print_progress_bar(static_cast<double>(file_index) / text_files.size());
            }

            //text_files.clear();

            //results_file.close();

            // Reopen the results file for reading
            std::ifstream input_file(folder_name + "_Results.txt");
            std::string line;
            // optimal_count = 0;
            // near_optimal_count = 0;

            // while (std::getline(input_file, line)) {
            //     if (line.find("Optimal solution found") != std::string::npos) {
            //         optimal_count++;
            //     } else if (line.find("Near-optimal solution found") != std::string::npos) {
            //         near_optimal_count++;
            //     }
            // }

            input_file.close();

            // Print the number of optimal and near-optimal solutions for the current folder
            std::cout << "Folder: " << folder_name << std::endl;
            std::cout << "Optimal solutions: " << optimal_count << std::endl;
            std::cout << "Near-optimal solutions: " << near_optimal_count << std::endl << std::endl;
            final_results_ILS << "Folder: " << folder_name << std::endl;
            final_results_ILS << "Optimal solutions: " << optimal_count << std::endl;
            final_results_ILS << "Near-optimal solutions: " << near_optimal_count << std::endl << std::endl;

            arrResults[arrRowCount][0] = optimal_count; // ILS optimal results for folder
            arrResults[arrRowCount][1] = near_optimal_count; //ILS near optimal results for folder

            totTimePerFolder = totTimePerFolder/text_files.size();
            arrTime[arrRowCount][0] = totTimePerFolder; // ILS runtime for folder
            totalRunTimeILS = totalRunTimeILS + totTimePerFolder;
            totTimePerFolder = 0;
            arrRowCount++;

            optimal_count = 0;
            near_optimal_count = 0;
        }
        arrRowCount = 0;
    }
    optimal_count = 0;
    near_optimal_count = 0;

    use_best_first_search = true;
    use_hill_climbing = false;
    use_simulated_annealing = false;
    use_greedy_algorithm = false;


    use_first_fit = false;
    use_next_fit = false;
    use_best_fit = false;

    //good for 1D bin packing algorithm apparently
    use_first_fit_decreasing = false;
    use_best_fit_decreasing = true;   
    use_next_fit_decreasing = false;
    use_worst_fit_decreasing = false;  //try a different combo for tabu...


       /* TABU SEARCH ... */
     {
        for (int folder_index = 0; folder_index < folder_names.size(); ++folder_index) {
            std::string folder_name = folder_names[folder_index];
            std::string csv_file = csv_files[folder_index];
            read_known_optimal_solutions(csv_file);

            // Open the results file for writing
            //std::ofstream results_file(folder_name + "_Tabu" + "_Results.txt");

            for (const auto &entry : known_optimal_solutions) {
                std::cout << "File: " << entry.first << ", Known optimal value: " << entry.second << std::endl;
            } std::cout << "Known optimal solutions read. Running the algorithm now... " << std::endl << std::endl;
            count = 0;

            cout << "Heuristics used: " << heuristicList << endl;
            //results_file << "Algorithm used: Tabu Search" << endl;
            //results_file << "Heuristics used: " << heuristicList << endl << endl;

            std::vector<std::string> text_files = get_text_files(folder_name);

            for (int file_index = 0; file_index < text_files.size(); ++file_index) {
                count++;
                cout << "iteration #" << count << " out of " << text_files.size() << endl;
                std::string file_path = folder_name + "/" + text_files[file_index];
                Data data = read_file_data(file_path);

                auto start_time = std::chrono::high_resolution_clock::now();
                int num_iterations = 750;  // Adjust as needed
                int tabu_list_size = 150;
                int k = 15;
                int result = tabu_search(data, num_iterations, tabu_list_size, k);
                auto end_time = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> runtime = end_time - start_time;

                std::string optimal_value_color;
                optStatus = "";
                if (result == known_optimal_solutions[text_files[file_index].substr(0, text_files[file_index].size() - 4)]) {
                    optimal_value_color = "\033[32m";  // Green
                    optStatus += "Optimal solution found :)";
                    optimal_count++;
                } else if (result == known_optimal_solutions[text_files[file_index].substr(0, text_files[file_index].size() - 4)]+1) {
                    optimal_value_color = "\033[33m";  // Yellow
                    optStatus += "Near-optimal solution found :|";
                    near_optimal_count++;
                } else {
                    optimal_value_color = "\033[31m";  // Red
                    optStatus += "Bad solution found :(";
                }

                //results_file << "File: " << text_files[file_index] << std::endl;
                //results_file << "Runtime: " << runtime.count() << " s" << std::endl;
                //results_file << "Calculated Optimal: " << optStatus << " - " << result << std::endl;
                //results_file << "Known Optimal: " << known_optimal_solutions[text_files[file_index]] << std::endl << std::endl;


                std::cout << "File: " << text_files[file_index] << endl;
                cout<< "Runtime: \033[34m" << runtime.count() << "s\033[0m " << endl;
                totTimePerFolder += runtime.count();
                cout << optimal_value_color << optStatus << "\033[0m " << endl;
                cout << "Calculated Optimal: " << optimal_value_color << result << "\033[0m " << endl;
                cout << "Known Optimal: " << known_optimal_solutions[text_files[file_index].substr(0, text_files[file_index].size() - 4)] << std::endl << endl;

                //print_progress_bar(static_cast<double>(file_index) / text_files.size());
            }
            //text_files.clear();

            //results_file.close();

            // Reopen the results file for reading
            std::ifstream input_file(folder_name + "_Results.txt");
            std::string line;
            // optimal_count = 0;
            // near_optimal_count = 0;

            // while (std::getline(input_file, line)) {
            //     if (line.find("Optimal solution found") != std::string::npos) {
            //         optimal_count++;
            //     } else if (line.find("Near-optimal solution found") != std::string::npos) {
            //         near_optimal_count++;
            //     }
            // }

            input_file.close();

            // Print the number of optimal and near-optimal solutions for the current folder
            std::cout << "Folder: " << folder_name << std::endl;
            std::cout << "Optimal solutions: " << optimal_count << std::endl;
            std::cout << "Near-optimal solutions: " << near_optimal_count << std::endl << std::endl;
            final_results_Tabu << "Folder: " << folder_name << std::endl;
            final_results_Tabu << "Optimal solutions: " << optimal_count << std::endl;
            final_results_Tabu << "Near-optimal solutions: " << near_optimal_count << std::endl << std::endl;

            arrResults[arrRowCount][2] = optimal_count; // ILS optimal results for folder
            arrResults[arrRowCount][3] = near_optimal_count; //ILS near optimal results for folder

            totTimePerFolder = totTimePerFolder/text_files.size();
            arrTime[arrRowCount][1] = totTimePerFolder; // ILS runtime for folder
            totalRunTimeTabu = totalRunTimeTabu + totTimePerFolder;
            totTimePerFolder = 0;
            arrRowCount++;
            
            optimal_count = 0;
            near_optimal_count = 0;
        }
        arrRowCount = 0;
    }
   
    //compute totals in 2d array -> add rows 
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 9; i++) {
            //goes from row 0 to 8, sum goes in row 9
            sum += arrResults[i][j];
        }
        arrResults[9][j] = sum;
        sum = 0;
    }
    sum = 0;

    final_results_table_1 << "Folder" << "\t\t\t" << "ILS Optimal" << "\t\t" << "ILS Near Optimal" << "\t\t" << "Tabu Optimal" << "\t\t" << "Tabu Near Optimal" << endl;
    final_results_table_1 << "-----------------------------------------------------------------------------------------------------------------------------" << endl;
    for (int i = 0; i < 9; i++) {
        strResult = strResult+ strResults[i];
        cout << "i is: " << i << " and :::: " << strResults[i] << " helloooo " << endl;
        if (i == 1) {
            strResult += "\t\t\t";
            for (int j = 0; j < 4; j++) {
                strResult += to_string(arrResults[i][j]) + "\t\t\t\t"; 
            } //10th row is totals
        } else {
            for (int j = 0; j < 4; j++) {
                strResult += "\t\t" + to_string(arrResults[i][j]) + "\t\t"; 
            } //10th row is totals
        }
        final_results_table_1 << strResult << endl;
        strResult = "";
    }
    final_results_table_1 << "-----------------------------------------------------------------------------------------------------------------------------" << endl;
    strResult += "Totals\t\t\t";
    for (int j = 0; j < 4; j++) {
        strResult += to_string(arrResults[9][j]) + "\t\t\t\t";
    }
    final_results_table_1 << strResult << endl;
    cout << strResult << endl;
    strResult = "";
        
    final_results_table_2 << "Folder" << "\t\t\t" << "ILS Runtime" << "\t\t\t" << "Tabu Runtime" << endl;
    final_results_table_2 << "-----------------------------------------------------------------------------------------------------------------" << endl;
    for (int i = 0; i < 9; i++) {
        strResult += strResults[i]; 
        cout << strResult << endl;  
        if (i == 1) {
            for (int j = 0; j < 2; j++) {
                strResult += "\t\t\t" + to_string(arrTime[i][j]);
            }
        } else if (i == 0 || i == 2 || i == 3) {
            for (int j = 0; j < 2; j++) {
                strResult += "\t\t" + to_string(arrTime[i][j]) + "\t";
            }
        } 
        else {
            for (int j = 0; j < 2; j++) {
                if (j == 0) {
                    strResult += "\t\t" + to_string(arrTime[i][j]);
                } else {
                    strResult += "\t\t\t" + to_string(arrTime[i][j]);
                }
            }
        }
        final_results_table_2 << strResult << endl;
        strResult = "";
    }
    final_results_table_2 << "-----------------------------------------------------------------------------------------------------------------" << endl;
    strResult += "Totals";

    strResult += "\t\t\t" + to_string(totalRunTimeILS) + "\t\t\t" + to_string(totalRunTimeTabu);
    
    final_results_table_2 << strResult << endl;
    cout << strResult << endl;
    strResult = "";
    sum = 0;
    
    final_results_ILS.close();
    final_results_Tabu.close();

    return 0;
}