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
        return DFA(d.get_transitions(), d.get_finaly_states(), d.get_start_state(), d.get_alphabet(), d.get_all_states(), d.get_deadlock());
    }
    std::map<std::pair<std::pair<int, int>, char>, std::pair<int, int>> mul;
    std::unordered_set<char> new_alphabet;
    int new_start_state;
    std::map<std::pair<int, int>, int> final_automat; //ключ-состояние в мульти автомате, значение - состояние в новом автомате
    std::map<std::pair<int, char>, int> new_transition_map;

    //эквивалентность
    std::set<int> new_final_states_1;

    //пересечение
    std::set<int> new_final_states_2;

    //инверсия


public:
    Regex(std::string str) : automat(buildDFA(str)) {}
    void compile(std::string str) { automat = buildDFA(str); }
    bool match(std::string& str) { return automat.match(str); }
    std::string recoveryy() { return automat.recovery(); }
    bool equile(Regex& r2);
    void mul_automats(Regex& r2);
    bool DFS(std::map<std::pair<int, char>, int>& ntm, std::set<int>& fs, int ss, std::set<int>& v);
    void intersection(Regex& r2);
    void print() {automat.printDFA();}
    std::string inverse(std::string& str);
    void postorder_for_inversion(std::unique_ptr<Node> &top);
};