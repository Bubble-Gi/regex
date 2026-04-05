#include "regex.h"

void Regex::mul_automats(Regex& r2) {
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
            if (automat.transitionss().contains({fs.first.first,ch}) && r2.automat.transitionss().contains({fs.first.first,ch})) {
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
    return DFS(new_transition_map, new_final_states_1, new_start_state, vis);
}

bool Regex::DFS(std::map<std::pair<int, char>, int>& ntm, std::set<int>& fs, int ss, std::set<int>& v) {
   if (fs.contains(ss)) return false;
    v.insert(ss);
    for (auto ch : new_alphabet) {
        if (ntm.contains({ss,ch})) {
            if (!v.contains(ntm[{ss,ch}])){
                if (!DFS(ntm,fs,ntm[{ss,ch}],v))
                return false;
            }
        }
    }
    return true;
}

void Regex::intersection(Regex& r2) {
    mul_automats(r2);
    size_t i = 0;
    for (auto& fs : mul) {
        if (automat.finaly_statess().contains(fs.first.first.first) && r2.automat.finaly_statess().contains(fs.first.first.second)) {
            new_final_states_2.insert(i);
            }
        i++;
    }
}

std::string Regex::inverse(std::string& str) {
    DFABuilder d(true);
    d.make_tree(str);
    d.makeDFA();
    d.minimization_by_Hopcroft();
    DFA a(d.get_transitions(), d.get_finaly_states(), d.get_start_state(), d.get_alphabet(), d.get_all_states(), d.get_deadlock());
    return a.recovery();
}


// void Regex::postorder_for_inversion(std::unique_ptr<Node> &top) {
//     if (!top) return;
//     postorder_for_inversion(top->left);
//     postorder_for_inversion(top->right);
//     char ch = top->type;
//     if (ch == CAT) {
//         std::unique_ptr<Node> tmp = std::move(top->left);
//         top->left = std::move(top->right);
//         top->right = std::move(tmp);
//     }
// }

