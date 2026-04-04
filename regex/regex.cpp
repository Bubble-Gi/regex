#include "regex.h"

void Regex::mul_automats(Regex& r2) {
    new_alphabet = automat.alphabets();
    for (auto ch : r2.automat.alphabets()) new_alphabet.insert(ch);
    for (auto ch : new_alphabet) {
        for (auto& q : automat.transitionss()) {
            for (auto& p : r2.automat.transitionss()) {
                if (automat.transitionss().contains({q.first.first,ch})
                    && r2.automat.transitionss().contains({p.first.first,ch})) {
                    mul[{{q.first.first,p.first.first},ch}] = {q.second, p.second};
                    if (q.first.first == automat.start_states() && p.first.first == r2.automat.start_states()) {
                        new_start_state = {q.first.first,p.first.first};
                    }
                }
            }
        }
    }
}

bool Regex::equile(Regex &r2) {
    //для автомата
    size_t i = 0;
    std::map<std::pair<int, int>, int> final_automat; //ключ-состояние в мульти автомате, значение - состояние в новом автомате
    std::map<std::pair<int, char>, int> new_transition_map;
    std::set<int> new_final_states;
    mul_automats(r2);
    for (auto& fs : mul) {
        final_automat[fs.first.first] = i;
        if (automat.finaly_statess().contains(fs.first.first.first) && !r2.automat.finaly_statess().contains(fs.first.first.second)
            || !automat.finaly_statess().contains(fs.first.first.first) && r2.automat.finaly_statess().contains(fs.first.first.second)) {
            new_final_states.insert(i);
        }
        i++;
    }
    for (auto& fs : mul) {
        new_transition_map[{final_automat[fs.first.first],fs.first.second}] = final_automat[fs.second];
    }
    std::set<int> vis;
    return DFS(new_transition_map, new_final_states, final_automat[new_start_state], vis);

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

