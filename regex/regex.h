#pragma once
#include "../DFA/minDFA.h"
#include <string>
#include "../tree/DFA.h"
class Regex {
    bool is_lookahead = false;
    DFA automat;
    DFA buildDFA(std::string& str) {
        DFABuilder d;
        DFABuilder d2;
        std::unique_ptr<DFA> l = nullptr;
        std::pair<std::string, std::string> result = lookahead(str);
        d.make_tree(result.first);
        d.makeDFA();
        d.minimization_by_Hopcroft();

        if (!result.second.empty()) is_lookahead = true;
        if (is_lookahead) {
            d2.make_tree(result.second);
            d2.makeDFA();
            d2.minimization_by_Hopcroft();
            l = std::make_unique<DFA>(d2.get_transitions(), d2.get_finaly_states(), d2.get_start_state(), d2.get_alphabet(),
                   d2.get_all_states(), d2.get_deadlock());
        }
        return DFA(d.get_transitions(), d.get_finaly_states(), d.get_start_state(), d.get_alphabet(),
                   d.get_all_states(), d.get_deadlock(), std::move(l));
    }

    // DFA buildDFA(std::string& str) {
    //     DFABuilder d;
    //     d.make_tree(str);
    //     d.makeDFA();
    //     //d.printDFA();
    //     d.minimization_by_Hopcroft();
    //     return DFA(d.get_transitions(), d.get_finaly_states(), d.get_start_state(), d.get_alphabet(), d.get_all_states(), d.get_deadlock());
    // }
    std::map<std::pair<std::pair<int, int>, char>, std::pair<int, int>> mul;
    std::unordered_set<char> new_alphabet;
    int new_start_state;
    std::map<std::pair<int, int>, int> final_automat; //ключ-состояние в мульти автомате, значение - состояние в новом автомате
    std::map<std::pair<int, char>, int> new_transition_map;

    //эквивалентность
    std::set<int> new_final_states_1;

    //пересечение
    std::set<int> new_final_states_2;



public:
    Regex(std::string str) : automat(buildDFA(str)) {}
    Regex(DFA a) : automat(std::move(a)) {}
    void compile(std::string str) { automat = buildDFA(str); }
    bool match(std::string str) { return automat.match(str); }
    std::string recoveryy() { return automat.recovery(); }
    bool equile(Regex& r2);
    void mul_automats(Regex& r2);
    bool DFS(std::map<std::pair<int, char>, int>& ntm, std::set<int>& fs, int ss, std::set<int>& v);
    Regex intersection(Regex& r2);
    void print() {automat.printDFA();}
    std::string inverse(std::string& str);
    void postorder_for_inversion(std::unique_ptr<Node> &top);

    std::pair<std::string, std::string> lookahead(std::string str);
};