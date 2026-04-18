#include "DFA.h"
#include "tree.h"
#include <iostream>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <iostream>

void DFABuilder::makeDFA() {
    int count = 0; //счетчик состояний
    help_set[t.get_root()->firstpos] = 0;
    count++;
    all_states.insert(help_set[t.get_root()->firstpos]);
    dont_mark_state.push(t.get_root()->firstpos);
    if (t.get_root()->firstpos.contains(t.get_pos())) finaly_states.insert(0);
    while (!dont_mark_state.empty()) {
        std::set<int> S = dont_mark_state.front();
        for (auto a : t.get_alphabet()) {
            std::set<int> U;
            for ( auto& p : S) {
                if (t.get_char_position()[p] == a) {
                    U.insert(t.get_followpos()[p].begin(), t.get_followpos()[p].end());
                }
            }
            if (help_set.find(U) == help_set.end()) {
                dont_mark_state.push(U);
                help_set[U] = count++;
                all_states.insert(help_set[U]);
                if (U.find(t.get_pos()) != U.end()) {
                    finaly_states.insert(help_set[U]);
                }
            }
            transitions[std::make_pair(help_set[S], a)] = help_set[U];
        }
        dont_mark_state.pop();
    }
    draw_original_dfa("max.dot");
}

void DFABuilder::draw_original_dfa(const std::string& filename) {
    std::ofstream out(filename);
    out << "digraph OriginalDFA {\n";
    out << "    rankdir=LR;\n";
    out << "    node [shape = circle];\n";
    for (int s : finaly_states) {
        out << "    " << s << " [shape = doublecircle];\n";
    }
    for (auto const& [key, target] : transitions) {
        int from = key.first;
        char c = key.second;
        out << "    " << from << " -> " << target << " [label = \"" << c << "\"];\n";
    }

    out << "}\n";
}

void DFABuilder::minimization_by_Hopcroft() {
    not_final_states = difference_sets(all_states, finaly_states);
    if (!finaly_states.empty()) P.push_back(finaly_states);
    if (!not_final_states.empty()) P.push_back(not_final_states);
    if (!finaly_states.empty()) W.push_back(finaly_states);
    if (!not_final_states.empty()) W.push_back(not_final_states);

    while (!W.empty()) {
        std::set<int>A = W.front();
        W.pop_front();
        for (auto c : t.get_alphabet()) {
            std::set<int> X;
            for (auto q : all_states) {
                if (transitions.find({q, c}) !=  transitions.end()) {
                    if (A.count(transitions[{q, c}])) X.insert(q);
                }
            }
            std::vector<std::set<int>> new_P;
            for (auto& Y: P) {
                std::set<int> is = intersection_sets(Y, X);
                std::set<int> ds = difference_sets(Y, X);
                if (!is.empty() && !ds.empty()) {
                    new_P.push_back(is);
                    new_P.push_back(ds);
                    if (std::find(W.begin(), W.end(), Y) != W.end()) {
                        W.erase(std::find(W.begin(), W.end(), Y));
                        W.push_back(is);
                        W.push_back(ds);
                    } else {
                        if (is.size() <= ds.size()) W.push_back(is);
                        else W.push_back(ds);
                    }
                } else new_P.push_back(Y);
            }
            P = std::move(new_P);
        }
    }
    std::map<int, int> tr;
    for (int i = 0; i < P.size(); i++) {
        for (auto p : P[i]) {
            tr[p] = i;
        }
        if (finaly_states.contains(*P[i].begin())) new_finaly_states.insert(i);
    }
    for (int i = 0; i < P.size(); i++) {
        for (auto ch : get_alphabet()) {
            new_map_tran[{i,ch}] = tr[transitions[{*P[i].begin(),ch}]];
        }
    }
    start_state = tr[0];

    all_states.clear();
    for (int i = 0; i < P.size(); i++) {
        all_states.insert(i);
    }

    for (auto p : all_states) {
        bool f = false;
        for (auto ch : get_alphabet()) {
            if (new_map_tran.contains({p,ch})) {
                if (new_map_tran[{p, ch}] != p) {
                    f = true;
                }
            }
        }
        if (!f) deadlock_state = p;
    }
    draw_minimized_dfa("min.dot");
}


// void DFABuilder::printDFA(){
//     for (auto& p : help_set) {
//         std::cout << p.second << " {";
//         for (auto a : p.first) {
//             std::cout << a << ",";
//         }
//         std::cout << "} ";
//         if (finaly_states.contains(p.second)) std::cout << "true" << std::endl;
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

std::set<int> DFABuilder::intersection_sets(std::set<int>& A, std::set<int>& B) {
    std::set<int> new_set;
    std::set_intersection(A.begin(), A.end(), B.begin(), B.end(),std::inserter(new_set, new_set.begin()));
    return new_set;
}

std::set<int> DFABuilder::difference_sets(std::set<int>& A, std::set<int>& B) {
    std::set<int> new_set;
    std::set_difference(A.begin(), A.end(), B.begin(), B.end(),std::inserter(new_set, new_set.begin()));
    return new_set;
}

void DFABuilder::draw_minimized_dfa(const std::string& filename) {
    std::ofstream out(filename);
    if (!out.is_open()) return;

    out << "digraph MinimizedDFA {\n";
    out << "    rankdir=LR;\n";
    out << "    node [fontname=\"Helvetica\", shape=circle];\n";
    out << "    edge [fontname=\"Helvetica\"];\n";

    // 1. Входная стрелка для начального состояния
    out << "    start [shape=none, label=\"\"];\n";
    out << "    start -> " << start_state << ";\n";

    // 2. Отрисовка НОВЫХ состояний (индексов в P)
    for (size_t i = 0; i < P.size(); ++i) {
        // Проверяем финальность по новому сету
        bool is_final = new_finaly_states.count(i);

        // Формируем метку, показывающую состав группы состояний
        std::string label = "Node " + std::to_string(i) + "\\n{";
        for (auto it = P[i].begin(); it != P[i].end(); ++it) {
            label += std::to_string(*it) + (std::next(it) != P[i].end() ? "," : "");
        }
        label += "}";

        out << "    " << i << " [shape=" << (is_final ? "doublecircle" : "circle")
            << ", label=\"" << label << "\""
            << (i == start_state ? ", color=blue, penwidth=2" : "") << "];\n";
    }

    // 3. Группировка переходов (чтобы не было лишних стрелок)
    // Map: {откуда, куда} -> "символ1, символ2..."
    std::map<std::pair<int, int>, std::string> aggregated_edges;
    for (auto const& [key, target] : new_map_tran) {
        int from = key.first;
        char symbol = key.second;

        if (aggregated_edges[{from, target}].empty()) {
            aggregated_edges[{from, target}] = std::string(1, symbol);
        } else {
            aggregated_edges[{from, target}] += ", " + std::string(1, symbol);
        }
    }

    // 4. Отрисовка сгруппированных переходов
    for (auto const& [nodes, labels] : aggregated_edges) {
        out << "    " << nodes.first << " -> " << nodes.second
            << " [label=\"" << labels << "\"];\n";
    }

    out << "}\n";
    out.close();
}