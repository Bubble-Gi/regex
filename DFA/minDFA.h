#pragma once
#include <map>
#include <set>
#include <unordered_set>
#include <string>
#include <fstream>
#include <iostream>

class DFA {
    std::map<std::pair<int, char>, int> transitions; // состояние, символ, куда переходим
    std::set<int> finaly_states;
    int start_state = 0;
    std::unordered_set<char> alphabet;
    std::set<int> all_states;

    //инверсия
    //std::map<std::pair<int, char>, int> new_transitions;

    //восстановление
    std::map<std::pair<int, std::string>, int> recovery_transitions;

public:
    DFA(std::map<std::pair<int, char>, int>& transitions, std::set<int>& f_states, int& s, std::unordered_set<char>& alph, std::set<int>& states) : transitions(std::move(transitions)), finaly_states(std::move(f_states)), start_state(s), alphabet(std::move(alph)), all_states(std::move(states)) {}
    bool match(std::string& str);

    //восстановление
    std::string recovery();

    //инверсия
    // void reverse_transitions();
    // std::set<int> new_start_state() { return finaly_states; }
    // int new_finaly_states() const { return start_state; }
    //
    // void draw_inverse_dfa(const std::string& filename) {
    //     std::ofstream out(filename);
    //     reverse_transitions();
    //     out << "digraph InverseDFA {\n";
    //     out << "    rankdir=LR;\n";
    //     out << "    node [shape = circle];\n";
    //     //for (int s : new_start_state()){
    //         out << "    " << new_finaly_states() << " [shape = doublecircle];\n";
    //     //}
    //     for (auto const& [key, target] : new_transitions) {
    //         int from = key.first;
    //         char c = key.second;
    //         out << "    " << from << " -> " << target << " [label = \"" << c << "\"];\n";
    //     }
    //
    //     out << "}\n";
    // }
};