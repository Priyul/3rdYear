Functions in program:


* Reading data functions:
	- print_progress_bar(double progress): Displays a progress bar, updating it based on the given progress of the search algorithm through any given (single) text file.

	- read_known_optimal_solutions(const std::string &csv_file): Reads known optimal solutions from a CSV file and stores them in a map.

	- get_text_files(const std::string &folder_name): Returns a vector of text file names in the given folder.

	- read_file_data(const std::string &file_path): Reads data from a text file and returns a Data structure containing problem information.

* Heuristics implemented:

	- best_first_search(const Data& data): Heuristic that sorts items in descending order based on their size.
	
	- hill_climbing(const Data& data): Heuristic that sorts items in ascending order based on their size.
	
	- simulated_annealing(Data &data, double start_temp, double end_temp, double cooling_rate): Simulated annealing heuristic that perturbs the item order based on a temperature schedule.
    
    - greedy_algorithm(const Data &data): Heuristic that sorts items in descending order based on their size.
	
	- generateHeuristicList(): Generates a string representing the list of heuristics used.

* Base ILS algorithm functions:
	- first_fit(const Data& data): Applies the First Fit algorithm to the given data and returns the number of bins used.
	
	- perturbation(std::vector<int>& items, int num_perturbations): Randomly swaps item positions in the vector.
	
	- local_search(const Data& data, std::vector<int>& items): Performs a local search on the items, attempting to find a better solution by swapping item positions.

	- iterated_local_search(Data& data, int num_iterations, int num_perturbations): Performs iterated local search, applying perturbation and local search repeatedly to find better solutions.

* Base Tabu Search function:
	- tabu_search(): Performs tabu search - it is a metaheuristic optimisation algorithm that helps find better solutions by exploring the search space while avoiding local optima. The algorithm introduces a "tabu list" to remember the most recent solutions and prevent revisiting them. When searching for the next solution, the algorithm uses the aspiration criterion to allow moves even if they are in the tabu list, which helps escape local optima.

The main difference between Tabu Search and Iterated Local Search (ILS) is the way they handle local optima. ILS uses random perturbations to escape local optima, whereas Tabu Search maintains a memory (tabu list) to prevent cycling back to previous solutions.

** Main:
	- main(): The main function reads input files, sets heuristics, and runs the iterated local search algorithm, outputting results to a file.



** Possible future heuristic implementations optimised for the 1D bin packing problem:
1. First Fit (FF): Place each item in the first bin that can accommodate it.
2. Best Fit (BF): Place each item in the tightest bin that can accommodate it, minimising the remaining space.
3. Next Fit (NF): Place each item in the current open bin if it fits; otherwise, close the bin and open a new one.
4. First Fit Decreasing (FFD): Sort the items in non-increasing order and apply the First Fit heuristic.
5. Best Fit Decreasing (BFD): Sort the items in non-increasing order and apply the Best Fit heuristic.
