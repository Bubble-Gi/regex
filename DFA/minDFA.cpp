#include "minDFA.h"
#include <iostream>
#include <queue>

bool DFA::match(std::string& str) {
    int state = start_state;
    for (auto c : str) {
        if (alphabet.find(c) != alphabet.end()) {
            state = transitions[{state,c}];
            std::cout << state << std::endl;
        } else {
            return false;
        }
    }
    return finaly_states.contains(state);
}

std::string DFA::recovery() {
    for (auto old : transitions) {
        recovery_transitions.insert({{old.first.first, old.second}, std::string(1,old.first.second)});
    }
    int new_start = -1;
    int new_end = all_states.size()+1;
    recovery_transitions[{new_start, start_state}] = "$";
    for (auto& a : finaly_states) recovery_transitions[{a, new_end}] = "$";

    std::set<int> states_and_qs = all_states;
    states_and_qs.insert(new_start);
    std::set<int> states_and_qf = all_states;
    states_and_qf.insert(new_end);

    std::queue<int> del_state;
    for (auto i : all_states) del_state.push(i);

    while (!del_state.empty()) {
        int state = del_state.front();
        for (auto i : states_and_qs) {
            if (recovery_transitions.contains({i, state})) {
                for (auto j : states_and_qf) {
                    if (recovery_transitions.contains({state, j})) {
                        std::string R_ik = recovery_transitions[{i, state}];
                        std::string R_kj = recovery_transitions[{state, j}];

                        std::string R_kk = recovery_transitions.contains({state, state}) ? recovery_transitions[{state, state}] : "";
                        std::string R_ij = recovery_transitions.contains({i, j}) ? recovery_transitions[{i, j}] : "";

                        recovery_transitions[{i, j}] = "(" + R_ij + ")|(" + R_ik + "(" + R_kk + ")*" + R_kj + ")";
                    }
                }
            }
        }
        for (auto it = recovery_transitions.begin(); it != recovery_transitions.end(); ) {
            if (it->first.first == state || it->first.second == state) {
                it = recovery_transitions.erase(it);
            } else {
                ++it;
            }
        }
        states_and_qs.erase(del_state.front());
        states_and_qf.erase(del_state.front());
        del_state.pop();
    }
    return recovery_transitions[{new_start, new_end}];
}


