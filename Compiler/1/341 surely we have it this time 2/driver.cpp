#include <iostream>
#include <stack>
#include <map>
#include <set>
#include <vector>
#include <sstream>
#include <cctype>
#include <queue>

#include <algorithm> 
#include <queue> 
#include <unordered_map> 
#include <iterator>
#include <fstream>


using namespace std;

class State {
public:
    int id;
    map<char, set<State *> > transitions;

    explicit State(int id) : id(id) {}
};

class NFA {
public:
    State *start;
    State *accept;

    NFA(State *start, State *accept) : start(start), accept(accept) {}
};

int state_id = 0;
int dfa_state_id = 0;

bool is_valid_regex(const string &regex) {
    for (char c : regex) {
        if (!isalnum(c) && c != '+' && c != '?' && c != '|' && c != '*' && c != '(' && c != ')' && c != '~' && c != '.') {
            return false;
        }
    }
    return true;
}


NFA *concatenate(NFA *a, NFA *b) {
    a->accept->transitions['\0'].insert(b->start);
    return new NFA(a->start, b->accept);
}

NFA *alternate(NFA *a, NFA *b) {
    State *start = new State(state_id++);
    State *accept = new State(state_id++);

    start->transitions['\0'].insert(a->start);
    start->transitions['\0'].insert(b->start);
    a->accept->transitions['\0'].insert(accept);
    b->accept->transitions['\0'].insert(accept);

    return new NFA(start, accept);
}

NFA *kleene_star(NFA *nfa) {
    State *start = new State(state_id++);
    State *accept = new State(state_id++);

    start->transitions['\0'].insert(nfa->start);
    nfa->accept->transitions['\0'].insert(accept);
    nfa->accept->transitions['\0'].insert(nfa->start);

    start->transitions['\0'].insert(accept);

    return new NFA(start, accept);
}

NFA *plus_operator(NFA *nfa) {
    State* start = new State(state_id++);
    State* accept = new State(state_id++);

    start->transitions['\0'].insert(nfa->start);
    nfa->accept->transitions['\0'].insert(accept);
    nfa->accept->transitions['\0'].insert(nfa->start);

    NFA* newNFA = new NFA(start, accept);

    return newNFA;
}

NFA *regex_to_nfa(const string &regex, size_t &pos) {
    stack<NFA *> nfa_stack;
    stack<char> operator_stack;

    while (pos < regex.size()) {
        char c = regex[pos];
        if (c == '(') {
            operator_stack.push(c);
            pos++;
        } else if (c == ')') {
            while (!operator_stack.empty() && operator_stack.top() != '(') {
                char op = operator_stack.top();
                operator_stack.pop();
                if (op == '|') {
                    NFA *b = nfa_stack.top();
                    nfa_stack.pop();
                    NFA *a = nfa_stack.top();
                    nfa_stack.pop();
                    nfa_stack.push(alternate(a, b));
                } else if (op == '.') {
                    NFA *b = nfa_stack.top();
                    nfa_stack.pop();
                    NFA *a = nfa_stack.top();
                    nfa_stack.pop();
                    nfa_stack.push(concatenate(a, b));
                }
            }
            operator_stack.pop(); 
            pos++;
        } else if (c == '|') {
            operator_stack.push(c);
            pos++;
        } else if (c == '*') {
            NFA *nfa = nfa_stack.top();
            nfa_stack.pop();
            nfa_stack.push(kleene_star(nfa));
            pos++;
        } else if (c == '~') {
            State *start = new State(state_id++);
            State *accept = new State(state_id++);
            start->transitions['\0'].insert(accept);
            nfa_stack.push(new NFA(start, accept));
            pos++;
        }  else if (c == '+') {
            operator_stack.push('.');
            pos++;
        }   else if (c == '.') {
            operator_stack.push('.');
            pos++;
        } else {
            State *start = new State(state_id++);
            State *accept = new State(state_id++);
            start->transitions[c].insert(accept);
            nfa_stack.push(new NFA(start, accept));
            pos++;
        }
        if (pos < regex.size() && regex[pos] == '.') {
            operator_stack.push('.');
            pos++;
        }
    }

    while (!operator_stack.empty()) {
    char op = operator_stack.top();
    operator_stack.pop();
    if (op == '|') {
        NFA *b = nfa_stack.top();
        nfa_stack.pop();
        NFA *a = nfa_stack.top();
        nfa_stack.pop();
        nfa_stack.push(alternate(a, b));
    } else if (op == '.') {
        NFA *b = nfa_stack.top();
        nfa_stack.pop();
        NFA *a = nfa_stack.top();
        nfa_stack.pop();
        nfa_stack.push(concatenate(a, b));
    }
    }

    NFA *result = nfa_stack.top();
    nfa_stack.pop();
    return result;
}


void print_transitions(State *state, State *final_state, set<int> &visited) {
    if (visited.count(state->id) > 0) {
        return;
    }
    visited.insert(state->id);
    for (const auto &transition : state->transitions) {
        for (State *next_state : transition.second) {
            cout << state->id << " -> (" << (transition.first == '\0' ? "ε" : string(1, transition.first)) << ") -> "
                << next_state->id;
            if (next_state == final_state) {
                cout << "^final";
            }
            if (state == final_state) {
                cout << "^final";
            }
            cout << endl;
            print_transitions(next_state, final_state, visited);
        }
    }
}
 
void print_nfa_graph(NFA *nfa) { 
    cout << "Start state: " << nfa->start->id << endl;
    cout << "Accept state: " << nfa->accept->id << endl;
    set<int> visited;
    print_transitions(nfa->start, nfa->accept, visited);
}



void epsilon_closure(State *state, set<State *> &closure) {
    if (closure.find(state) != closure.end()) {
        return;
    }
    closure.insert(state);

    if (state->transitions.count('\0') > 0) {
        for (State *next_state : state->transitions.at('\0')) {
            epsilon_closure(next_state, closure);
        }
    }
}


class DFAState {
public:
    int id;
    set<State *> nfa_states;
    map<char, DFAState *> transitions;
    bool accept;

    explicit DFAState(int id, const set<State *> &nfa_states) : id(id), nfa_states(nfa_states), accept(false) {}
};

DFAState *nfa_states_transition(const set<State *> &nfa_states, char input) {
    set<State *> new_nfa_states;
    for (State *state : nfa_states) {
        if (state->transitions.count(input) > 0) {
            for (State *next_state : state->transitions.at(input)) {
                epsilon_closure(next_state, new_nfa_states);
            }
        }
    }
    return new DFAState(dfa_state_id++, new_nfa_states);
}


vector<DFAState *> get_dfa_states(DFAState *start) {
    vector<DFAState *> states = {start};
    set<int> visited;

    size_t current = 0;
    while (current < states.size()) {
        DFAState *state = states[current];

        if (visited.count(state->id) == 0) {
            visited.insert(state->id);

            for (const auto &transition : state->transitions) {
                DFAState *next_state = transition.second;

                if (visited.count(next_state->id) == 0) {
                    states.push_back(next_state);
                }
            }
        }
        ++current;
    }

    return states;
}


DFAState *nfa_to_dfa(NFA *nfa) {
   
     dfa_state_id = 0;
    set<State *> initial_nfa_states;
    epsilon_closure(nfa->start, initial_nfa_states);

    DFAState *start = new DFAState(state_id++, initial_nfa_states);
    vector<DFAState *> dfa_states = {start};
    size_t current = 0;

    while (current < dfa_states.size()) {
        DFAState *state = dfa_states[current];
        state->accept = state->nfa_states.count(nfa->accept) > 0;

        for (char input = ' '; input <= '~'; ++input) {
            if (input == '\0') {
                continue;
            }

            DFAState *next_state = nfa_states_transition(state->nfa_states, input);
            if (next_state->nfa_states.empty()) {
                delete next_state;
                continue;
            }

            auto existing_state = find_if(dfa_states.begin(), dfa_states.end(), [&](DFAState *s) {
                return s->nfa_states == next_state->nfa_states;
            });

            if (existing_state == dfa_states.end()) {
                dfa_states.push_back(next_state);
            } else {
                delete next_state;
                next_state = *existing_state;
            }

            state->transitions[input] = next_state;
        }

        ++current;
    }

    return start;
}


void print_dfa_graph(DFAState *start) {
    map<DFAState *, int> state_ids;
    vector<DFAState *> states = get_dfa_states(start);

    int id = 0;
    for (DFAState *state : states) {
        state_ids[state] = id++;
    }

    cout << "Start state: " << state_ids[start] << endl;
    cout << "Accept states:";
    for (DFAState *state : states) {
        if (state->accept) {
            cout << " " << state_ids[state];
        }
    }
    cout << endl;

    queue<DFAState *> q;
    set<DFAState *> visited;

    q.push(start);
    visited.insert(start);

    while (!q.empty()) {
        DFAState *state = q.front();
        q.pop();

        for (const auto &transition : state->transitions) {
            char input = transition.first;
            DFAState *next_state = transition.second;

            if (visited.find(next_state) == visited.end()) {
                visited.insert(next_state);
                q.push(next_state);
            }

            cout << state_ids[state] << " -> (" << input << ") -> " << state_ids[next_state];
            if (next_state->accept) {
                cout << "^final";
            }
            cout << endl;
        }
    }
}

vector<set<DFAState *> > partition(vector<DFAState *> &states) {
    vector<set<DFAState *> > partitions(2);

    for (DFAState *state : states) {
        if (state->accept) {
            partitions[1].insert(state);
        } else {
            partitions[0].insert(state);
        }
    }

    queue<pair<set<DFAState *>, char> > to_check;
    set<set<DFAState *> > in_queue;

    for (char input = ' '; input <= '~'; ++input) {
        if (input == '\0') {
            continue;
        }

        to_check.push({partitions[0], input});
        to_check.push({partitions[1], input});
        in_queue.insert(partitions[0]);
        in_queue.insert(partitions[1]);
    }

    while (!to_check.empty()) {
        pair<set<DFAState *>, char> partition_input = to_check.front();
        to_check.pop();

        map<set<DFAState *> *, set<DFAState *> > partitioned;

        for (DFAState *state : partition_input.first) {
            if (state->transitions.count(partition_input.second) == 0) {
                continue;
            }

            DFAState *next_state = state->transitions[partition_input.second];

            for (set<DFAState *> &partition : partitions) {
                if (partition.count(next_state) > 0) {
                    partitioned[&partition].insert(state);
                    break;
                }
            }
        }

        if (partitioned.empty()) {
            continue;
        }

        for (const auto &entry : partitioned) {
            set<DFAState *> &old_partition = *entry.first;
            set<DFAState *> new_partition = entry.second;

            for (DFAState *state : new_partition) {
                old_partition.erase(state);
            }

            partitions.push_back(new_partition);

            for (char input = ' '; input <= '~'; ++input) {
                if (input == '\0') {
                    continue;
                }

                if (in_queue.find(new_partition) == in_queue.end()) {
                    to_check.push({new_partition, input});
                    in_queue.insert(new_partition);
                }
            }
        }
    }

    return partitions;
}


DFAState *minimize_dfa(DFAState *start) {
    state_id = 0;
    dfa_state_id = 0;
    vector<DFAState *> states = get_dfa_states(start);
    vector<set<DFAState *> > partitions = partition(states);

    map<int, DFAState *> new_states;
    for (const auto &partition : partitions) {
        int new_id = state_id++;
        DFAState *new_state = new DFAState(new_id, set<State *>());
        new_states[new_id] = new_state;
        for (DFAState *old_state : partition) {
            old_state->id = new_id;
            if (old_state->accept) {
                new_state->accept = true;
            }
        }
    }

    for (const auto &partition : partitions) {
        DFAState *representative = *partition.begin();
        for (const auto &transition : representative->transitions) {
            char input = transition.first;
            DFAState *next_state = transition.second;
            int next_state_id = next_state->id;
            if (new_states.count(next_state_id) == 0) {
                new_states[next_state_id] = new DFAState(next_state_id, set<State *>());
            }
            new_states[representative->id]->transitions[input] = new_states[next_state_id];

        }
    }

    return new_states[start->id];
}

void print_dfa_graph_to_xml(DFAState *start) {
    ofstream outfile;
    outfile.open("minDFA.xml", ios::out);
    outfile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    outfile << "<minDFA>\n";

    map<DFAState *, int> state_ids;
    vector<DFAState *> states = get_dfa_states(start);

    int id = 0;
    for (DFAState *state : states) {
        state_ids[state] = id++;
    }

    outfile << "<states>\n";
    for (DFAState *state : states) {
        outfile << "<" << state_ids[state] << "/>\n";
    }
    outfile << "</states>\n";

    outfile << "<transitions>\n";
    queue<DFAState *> q;
    set<DFAState *> visited;

    q.push(start);
    visited.insert(start);

    while (!q.empty()) {
        DFAState *state = q.front();
        q.pop();

        for (const auto &transition : state->transitions) {
            char input = transition.first;
            DFAState *next_state = transition.second;

            if (visited.find(next_state) == visited.end()) {
                visited.insert(next_state);
                q.push(next_state);
            }

            outfile << "<" << state_ids[state] << ">\n";
            outfile << "<" << state_ids[next_state] << ">" << input << "</" << state_ids[next_state] << ">\n";
            outfile << "</" << state_ids[state] << ">\n";
        }
    }
    outfile << "</transitions>\n";
    outfile << "</minDFA>";
    outfile.close();
}


int main() {
    string regex;
    cout << "Enter a regex: ";
    cin >> regex;
    if (!is_valid_regex(regex)) {
        cerr << "Invalid regex. Please use only (a-z), (A-Z), (0-9), +, ?, |, *, (, ), and ε(~)." << endl;
        return 1;
    }

    
    size_t pos = 0;
    NFA *nfa = regex_to_nfa(regex, pos);
    cout << "NFA:" << endl;
    print_nfa_graph(nfa);

    cout << "==============================" << endl;
    cout << "DFA:" << endl;
    DFAState *dfa_start = nfa_to_dfa(nfa);
    print_dfa_graph(dfa_start);

    cout << "==============================" << endl;
    cout << "Minimized DFA:" << endl;
    DFAState *minimized_dfa_start = minimize_dfa(dfa_start);
    print_dfa_graph(minimized_dfa_start);
    print_dfa_graph_to_xml(minimized_dfa_start);

    delete nfa;
    delete dfa_start;
    delete minimized_dfa_start;

    return 0;
}
