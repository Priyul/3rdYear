This C++ implementation intends to solve the 1D bin packing problem by comparing the results of the Iterated Local Search (ILS) and Tabu Search algorithms. The code includes various heuristics and functions to find the most efficient solutions for the problem.

Function Explanations:

Utility Functions:
	- print_progress_bar(double progress): Displays a progress bar on the console, taking a progress value between 0 and 1.
	- read_known_optimal_solutions(const std::string &csv_file): Reads known optimal solutions from a CSV file and stores them in a map for comparison purposes.
	- get_text_files(const std::string &folder_name): Retrieves a list of text files from a specified folder.
	- read_file_data(const std::string &file_path): Reads the data from a file, including the number of items, bin capacity, and item sizes.

Bin Class:
	- Bin(int capacity): Constructor for the Bin class, initializing a bin with a specified capacity.
	-can_fit(int item_size) const: Checks if an item with a given size can fit into the bin.
	- add_item(int item_size): Adds an item with a given size to the bin.
	- remove_item(int index): Removes an item with a given index from the bin.
	- get_items() const: Returns the list of items in the bin.

Heuristics:
	- best_first_search(const Data& data): Sorts items in decreasing order of size.
	- hill_climbing(const Data& data): Sorts items in increasing order of size.
	- simulated_annealing(Data data, double start_temp, double end_temp, double cooling_rate): Applies the Simulated Annealing algorithm to reorder items.
	- greedy_algorithm(const Data &data): Sorts items in decreasing order of size.
	- next_fit(const Data& data): Implements the Next Fit heuristic.
	- best_fit(const Data& data): Implements the Best Fit heuristic.
	- first_fit_decreasing(const Data& data): Implements the First Fit Decreasing heuristic.

Iterated Local Search (ILS):
	- first_fit(const Data& data): Implements the First Fit heuristic.
	- perturbation(std::vector<int>& items, int num_perturbations): Applies perturbations to a given set of items.
	- local_search(const Data& data, std::vector<int>& items): Performs a local search using the specified heuristic.
	- iterated_local_search(Data& data, int num_iterations, int num_perturbations): Performs the Iterated Local Search algorithm.

Tabu Search:
	- delta(const std::vector<int>& a, const std::vector<int>& b): Calculates the delta between two solutions.
	- evaluate_solution(const Data& data, const std::vector<int>& item_order): Evaluates a given solution and returns the number of bins used.
	-neighborhood_search_tabu(const Data& data, const std::vector<int>& items, const std::deque<std::vector<int> >& tabu_list, int k, int best_result): Performs a neighborhood search in the Tabu Search algorithm.
	- tabu_search(Data& data, int num_iterations, int tabu_list_size, int k): Performs the Tabu Search algorithm.
