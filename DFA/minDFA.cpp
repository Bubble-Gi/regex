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

// void DFA::reverse_transitions() {
//     //std::cout << transitions.size() << std::endl;
//     for (auto& [st_c, tran] : transitions) {
//         int new_st_c = tran;
//         int new_tran = st_c.first;
//         new_transitions.insert({{new_st_c, st_c.second}, new_tran});
//     }
//     std::cout << new_transitions.size() << std::endl;
// }

std::string DFA::recovery() {
    std::queue<std::set<int>> delete_state;
    delete_state.push(all_states);

    while (!delete_state.empty()) {
        auto state = delete_state.front();
        if (state != finaly_states && !state.contains(start_state)) {

        }
        delete_state.pop();
    }
}


