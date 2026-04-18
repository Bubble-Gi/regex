#include "regex.h"

void Regex::mul_automats(Regex& r2) {
    new_transition_map.clear();
    final_automat.clear();
    size_t i = 0;
    new_alphabet = automat.alphabets();
    for (auto ch : r2.automat.alphabets()) new_alphabet.insert(ch);
    for (auto& q : automat.all_statess()) {
        for (auto& p : r2.automat.all_statess()) {
            final_automat[{q,p}] = i++;
        }
    }
    for (auto& fs : final_automat) {
        for (auto ch : automat.alphabets()) {
            if (automat.transitionss().contains({fs.first.first,ch}) && r2.automat.transitionss().contains({fs.first.second,ch})) {
                new_transition_map[{fs.second, ch}] = final_automat[{automat.transitionss()[{fs.first.first,ch}],r2.automat.transitionss()[{fs.first.second,ch}]}];
            }
        }
    }
    for (auto fs : final_automat) {
        if (fs.first.first == automat.start_states() &&  fs.first.second == r2.automat.start_states()) {
            new_start_state = fs.second;
        }
    }
}

bool Regex::equile(Regex &r2) {
    if (automat.alphabets() != r2.automat.alphabets()) return false;
    mul_automats(r2);
    for (auto& fs : final_automat) {
        if (automat.finaly_statess().contains(fs.first.first) && !r2.automat.finaly_statess().contains(fs.first.second)
            || !automat.finaly_statess().contains(fs.first.first) && r2.automat.finaly_statess().contains(fs.first.second)) {
            new_final_states_1.insert(fs.second);
        }
    }
    std::set<int> vis;
    bool res = DFS(new_transition_map, new_final_states_1, new_start_state, vis);
    final_automat.clear();
    return res;
}

bool Regex::DFS(std::map<std::pair<int, char>, int>& ntm, std::set<int>& fs, int ss, std::set<int>& v) {
   if (fs.contains(ss)) return false;
    v.insert(ss);
    for (auto ch : new_alphabet) {
        if (ntm.contains({ss,ch})) {
            if (!v.contains(ntm[{ss,ch}])){
                if (!DFS(ntm,fs,ntm[{ss,ch}],v)) return false;
            }
        }
    }
    return true;
}

Regex Regex::intersection(Regex& r2) {
    new_transition_map.clear();
    new_final_states_2.clear();
    final_automat.clear();
    mul_automats(r2);
    std::set<int> new_all;
    std::unordered_set<char> alf;
    for (auto ch1 : automat.alphabets()) {
        for (auto ch2 : r2.automat.alphabets()) {
            if (ch1 == ch2) alf.insert(ch1);
        }
    }
    for (auto& fs : final_automat) {
        new_all.insert(fs.second);
        if (automat.finaly_statess().contains(fs.first.first) && r2.automat.finaly_statess().contains(fs.first.second)) {
            new_final_states_2.insert(fs.second);
        }
    }
    //transitions(std::move(transitions)), finaly_states(std::move(f_states)), start_state(s), alphabet(std::move(alph)), all_states(std::move(states)), deadlock(dd)
    return DFA(new_transition_map, new_final_states_2, new_start_state, alf, new_all, 0);
}

std::string Regex::inverse(std::string& str) {
    DFABuilder d(true);
    d.make_tree(str);
    d.makeDFA();
    d.minimization_by_Hopcroft();
    DFA a(d.get_transitions(), d.get_finaly_states(), d.get_start_state(), d.get_alphabet(), d.get_all_states(), d.get_deadlock());
    return a.recovery();
}



std::pair<std::string, std::string> Regex::lookahead(std::string str) {
    int i = 0;
    std::string first;
    std::string last;
    for (int j = 0; j < str.length(); j++) {
        char ch = str[j];
        if (ch == '(') i++;
        if (ch == ')') i--;
        if (ch == '/') {
            if (i == 0) {
                first = str.substr(0, j);
                last = str.substr(j+1, str.length()-j-1);
                if (last.find("/") != std::string::npos) throw std::invalid_argument("2/");
                return std::make_pair(first, last);
            }
        }
    }
    return std::make_pair(str, "");
}
