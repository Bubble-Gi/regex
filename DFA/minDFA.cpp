#include "minDFA.h"
#include <iostream>
#include <queue>

bool DFA::match(std::string str) {
    int state = start_state;
    if (lookahead == nullptr) {
        for (auto c : str) {
            if (alphabet.find(c) != alphabet.end()) {
                state = transitions[{state,c}];
                //if (state == deadlock) { return false;}
            } else {
                return false;
            }
        }
        return finaly_states.contains(state);
    } else {
        for (int i = 0; i < str.length(); i++) {
            if (alphabet.find(str[i]) != alphabet.end()) {
                state = transitions[{state,str[i]}];
                if (finaly_states.contains(state)) {
                    if (lookahead->match(str.substr(i+1))) {
                        return true;
                    }
                }
            }
        }
        return false;
    }
}

std::string DFA::recovery() {
    if (lookahead != nullptr) throw std::runtime_error("восстановления не будэт");
    for (auto& old : transitions) {
        std::string s(1,old.first.second);
        if (recovery_transitions.contains({old.first.first, old.second})) {
            std::string t = recovery_transitions[{old.first.first, old.second}];
            recovery_transitions[{old.first.first, old.second}] = '(' + t + ")|(" + s + ')';
        } else {
            recovery_transitions[{old.first.first, old.second}] = s;
        }
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

                        //(R_ij)|R_ik(R_kk)*R_kj
                        std::string new_path;
                        new_path += "(" + R_ik + ")";
                        if (!R_kk.empty()) {
                            new_path += "(" + R_kk + ")*";
                        }
                        new_path += "(" + R_kj + ")";

                        std::string tmp;
                        if (R_ij.empty() || R_ij == "") {
                            tmp = new_path;
                        } else {
                            tmp = "(" + R_ij + ")|(" + new_path + ")";
                        }
                        recovery_transitions[{i, j}] = tmp;

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

// void DFA::printDFA(){
//     for (auto& p : all_states) {
//         std::cout << p << " {";
//         std::cout << "} ";
//         if (finaly_states.contains(p)) std::cout << "true" << std::endl;
//         else std::cout << "false" << std::endl;
//     }
//
//     for (auto& p : transitions) {
//         std::cout << p.first.first << " {";
//         std::cout << p.first.second << "->";
//         std::cout << p.second << "}" << std::endl;
//     }
//
//     for (auto& p : all_states) std::cout << p << ","; std::cout << std::endl;
//
//     std::cout << start_state << std::endl;
// }


