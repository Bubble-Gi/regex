#pragma once
#include "minDFA.h"
#include <string>
#include "DFA.h"
class Regex {
    DFA automat;
    DFA buildDFA(std::string& str) {
        DFABuilder d;
        d.make_tree(str);
        d.makeDFA();
        d.printDFA();
        d.minimization_by_Hopcroft();
        d.printDFA();
        return DFA(d.get_transitions(), d.get_finaly_states(), d.get_start_state(), d.get_alphabet(), d.get_all_states());
    }

public:
    Regex(std::string str) : automat(buildDFA(str)) {}
    void compile(std::string str) { automat = buildDFA(str); }
    bool match(std::string& str) { return automat.match(str); }
    //void get_size_tr() { automat.size_tr(); }
    //void get_inv_transit(const std::string& str) {automat.reverse_transitions(); automat.draw_inverse_dfa(str); }
};