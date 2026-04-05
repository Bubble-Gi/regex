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
    int deadlock;
    //восстановление
    std::map<std::pair<int, int>, std::string> recovery_transitions; //откуда, куда, что на ребре

public:
    DFA(std::map<std::pair<int, char>, int>& transitions, std::set<int>& f_states, int s, std::unordered_set<char>& alph, std::set<int>& states, int dd) : transitions(std::move(transitions)), finaly_states(std::move(f_states)), start_state(s), alphabet(std::move(alph)), all_states(std::move(states)), deadlock(dd) {}
    bool match(std::string& str);

    std::string recovery();
    std::map<std::pair<int, char>, int>& transitionss() { return transitions; }
    std::set<int>& finaly_statess() { return  finaly_states; }
    int start_states() { return start_state; }
    std::unordered_set<char> alphabets() { return  alphabet; }
    std::set<int>& all_statess() { return  all_states; }
    void printDFA();
    //int get_deadlock() {return deadlock;}
};