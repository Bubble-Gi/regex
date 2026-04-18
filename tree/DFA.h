#pragma once
#include <map>
#include <set>
#include "tree.h"
#include <queue>
#include <vector>
#include <list>

class DFABuilder{
    Tree t;
    std::map<std::pair<int, char>, int> transitions; //мапа для автомата
    std::map<std::set<int>, int> help_set; //вспомогательный сет переходов
    std::set<int> finaly_states; //множество принимающих состояний
    std::set<int> all_states; //множется всех состояний
    std::set<int> not_final_states;
    std::queue<std::set<int>> dont_mark_state; //очередь для помечайний обработанных состояний
    std::vector<std::set<int>> P; //разбиение
    std::list<std::set<int>> W; //очередь
    int start_state = 0;
    int deadlock_state;
    std::set<int> new_finaly_states;
    std::map<std::pair<int, char>, int> new_map_tran;
    bool is_inverse;

public:
    DFABuilder(bool is_inverse = false ) : is_inverse(is_inverse) {}
    void makeDFA();
    void printDFA();
    void make_tree(std::string str) { t.parsing(str, is_inverse); }
    void minimization_by_Hopcroft();
    std::set<int> intersection_sets(std::set<int>& A, std::set<int>& B);
    std::set<int> difference_sets(std::set<int>& A, std::set<int>& B);
    void draw_original_dfa(const std::string& filename);
    void draw_minimized_dfa(const std::string& filename);
    std::set<int>& get_finaly_states_before_min() {return finaly_states;}

    std::map<std::pair<int, char>, int>& get_transitions() { return new_map_tran;}
    std::set<int>& get_finaly_states() {return new_finaly_states;}
    int& get_start_state() { return start_state;}
    std::unordered_set<char>& get_alphabet() {return t.get_alphabet();}
    std::set<int>& get_all_states() { return all_states; }
    std::unique_ptr<Node>& get_roooot() { return t.get_unique_root();}
    int &get_deadlock() {return deadlock_state;}


};
