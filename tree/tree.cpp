#include "tree.h"
#include <cctype>
#include <iostream>
#include <string>
#include<fstream>

//1 самый низкий, 2 средний, 3 высокий
int Tree::priority(char &c) {
    switch (c) {
        case '|' :  return 1;
        case '.' : return 2;
        case '*' : return 3;
        case '+' : return 3;
        default: return 0;
    }
}

std::unique_ptr<Node> Tree::make_e_node(char ch) {
    auto n = std::make_unique<Node>();
    n->type = EPSILON;
    n->left = nullptr;
    n->right = nullptr;
    n->operand = ch;
    n->nullable = true;
    n->firstpos = {};
    n->lastpos = {};
    return n;
}

std::unique_ptr<Node> Tree::make_a_node(char ch) {
    auto n = std::make_unique<Node>();
    n->type = A;
    n->left = nullptr;
    n->right = nullptr;
    n->operand = ch;
    n->nullable = false;
    n->firstpos = {++pos};
    n->lastpos = {pos};
    alphabet.insert(ch);
    char_position[pos] = ch;
    return n;
}

std::unique_ptr<Node> Tree::make_or_node(char ch) {
    auto n = std::make_unique<Node>();
    n->type = OR;
    n->right = std::move(Operands.top());
    Operands.pop();
    n->left = std::move(Operands.top());
    Operands.pop();
    n->operand = ch;

    if (n->left->nullable == true || n->right->nullable == true) n->nullable = true;
    else n->nullable = false;

    n->firstpos = n->left->firstpos; n->firstpos.insert(n->right->firstpos.begin(), n->right->firstpos.end());
    n->lastpos = n->left->lastpos; n->lastpos.insert(n->right->lastpos.begin(), n->right->lastpos.end());
    return n;
}

std::unique_ptr<Node> Tree::make_cat_node(char ch) {
    auto n = std::make_unique<Node>();
    n->type = CAT;
    n->right = std::move(Operands.top());
    Operands.pop();
    n->left = std::move(Operands.top());
    Operands.pop();
    n->operand = ch;

    if (n->left->nullable == true && n->right->nullable == true) n->nullable = true;
    else n->nullable = false;

    if (n->left->nullable == true) { n->firstpos = n->left->firstpos; n->firstpos.insert(n->right->firstpos.begin(), n->right->firstpos.end()); }
    else n->firstpos = n->left->firstpos;

    if (n->right->nullable == true) { n->lastpos = n->left->lastpos; n->lastpos.insert(n->right->lastpos.begin(), n->right->lastpos.end()); }
    else n->lastpos = n->right->lastpos;

    for (auto i : n->left->lastpos) {
        followpos[i].insert(n->right->firstpos.begin(), n->right->firstpos.end());
    }
    return n;
}

std::unique_ptr<Node> Tree::make_star_node(char ch) {
    auto n = std::make_unique<Node>();
    n->type = STAR;
    n->left = nullptr;
    n->right = std::move(Operands.top());
    Operands.pop();
    n->operand = ch;

    n->nullable = true;
    n->firstpos = n->right->firstpos;
    n->lastpos = n->right->lastpos;

    for (auto i : n->right->lastpos) {
        followpos[i].insert(n->right->firstpos.begin(), n->right->firstpos.end());
    }

    return n;
}

std::unique_ptr<Node> Tree::make_plus_node(char ch) {
    auto n = std::make_unique<Node>();
    n->type = PLUS;
    n->left = nullptr;
    n->right = std::move(Operands.top());
    Operands.pop();
    n->operand = ch;

    if (n->right->nullable == true) n->nullable = true;
    else n->nullable = false;

    n->firstpos = n->right->firstpos;
    n->lastpos = n->right->lastpos;

    for (auto i : n->right->lastpos) {
        followpos[i].insert(n->right->firstpos.begin(), n->right->firstpos.end());
    }

    return n;
}

void Tree::parsing(std::string& str) {
    str.append(".#");
    for (size_t i = 0; i < str.length(); ++i) {
        char ch = str[i];
        if (isalpha(ch) || screening != 0) {
            Operands.push(std::move(make_a_node(ch)));
            screening = false;
        }

        else if (is_operator(ch)) {
            if (Operands.empty()) throw std::runtime_error("не может быть операция перед первым операндом");
            while (!Operators.empty() && priority(ch) <= priority(Operators.top()) && ch != '*' && ch != '.') {
                Operands.push(std::move(get_operator(Operators.top())));
                Operators.pop();
            }
            if (ch == '*') Operands.push(std::move(get_operator(ch)));

            if (ch == '.' && i+2  < str.length() && str[i+1]  == '.' && str[i+2]  == '.') {
                Operands.push(std::move(get_operator('*')));
                i += 3;
            } else if (ch == '.' && (str[i+1] != '.' || i++ >= str.length())) {
                while (!Operators.empty() && priority(ch) <= priority(Operators.top())) {
                    Operands.push(std::move(get_operator(Operators.top())));
                    Operators.pop();
                }
            }
            Operators.push(ch);
        }

        else if (ch == '(') {
            Operators.push(std::move(ch));
        }
        else if (ch == ')') {
            while (!Operators.empty() && Operators.top() != '(') {
                Operands.push(std::move(get_operator(Operators.top())));
                Operators.pop();
            }
            if (!Operators.empty() && Operators.top() == '(') Operators.pop();
        }
        else if (ch == '#') {
            Operands.push(std::move(make_a_node(ch)));
            while (!Operators.empty()) {
                Operands.push(std::move(get_operator(Operators.top())));
                Operators.pop();
            }
        } else if (ch == '%') screening = true;

        else if (ch == '[') {
            i++;
            std::string tmp;
            while (i != str.length() && str[i] != ']' ) tmp.push_back(str[i++]);
            if (i == str.length()) throw std::invalid_argument("] нет");
            make_range1(tmp);
        }

        else if (ch == '{') {
            i++;
            std::string tmp;
            while (i != str.length() && str[i] != '}' ) tmp.push_back(str[i++]);
            if (i == str.length()) throw std::invalid_argument("} нет");
            make_range2(tmp);
        }

        else if (ch == '$') {
            Operands.push(std::move(make_e_node(ch)));
        }
        else throw std::invalid_argument("не известный символ");
    }
    exportToGraphviz("id.dot", Operands.top());
    alphabet.erase('#');
    ptintFP();
}

bool Tree::is_operator(char& ch) {
    if (ch == '|' || ch == '*' || ch == '.' || ch == '+' || ch == '/') return true;
    return false;
}

std::unique_ptr<Node> Tree::get_operator(char ch) {
    switch (ch) {
        case '|': return make_or_node(ch);
        case '.': return make_cat_node(ch);
        case '*': return make_star_node(ch);
        case '+': return make_plus_node(ch);
        default:  throw std::runtime_error(" Invalid operator");
    }
}

std::string Tree::setToString(const std::set<int>& s) {
    if (s.empty()) return "empty";
    std::string res;
    for (int x : s) res += std::to_string(x) + " ";
    return res;
}

void Tree::writeNode(const std::unique_ptr<Node>& node, std::ostream& out) {
    if (!node) return;

    size_t nodeId = reinterpret_cast<size_t>(node.get());
    std::string op;
    if (node->operand == '|') op = "\\|";
    else if (node->operand == '{' || node->operand == '}') op = "\\" + std::string(1, node->operand);
    else op = node->operand;
    out << "    \"" << nodeId << "\" [label=\"{ "
        << "SYMBOL: " << op << " | "
        << "N: " << (node->nullable ? "true" : "false") << " | "
        << "F: [ " << setToString(node->firstpos) << "] | "
        << "L: [ " << setToString(node->lastpos) << "] "
        << "}\"];\n";
    if (node->left) {
        out << "    \"" << nodeId << "\" -> \"" << reinterpret_cast<size_t>(node->left.get()) << "\";\n";
        writeNode(node->left, out);
    }
    if (node->right) {
        out << "    \"" << nodeId << "\" -> \"" << reinterpret_cast<size_t>(node->right.get()) << "\";\n";
        writeNode(node->right, out);
    }
}

void Tree::exportToGraphviz(const std::string& filename, const std::unique_ptr<Node>& root) {
    std::ofstream out(filename);
    if (!out.is_open()) return;

    out << "digraph Tree {\n";
    out << "    node [fontname=\"Consolas\", shape=Mrecord];\n";

    if (root) writeNode(root, out);

    out << "}\n";
    out.close();
}

void Tree::ptintFP() const {
    for (auto [i, j] : followpos) {
        std::cout << i << ": " ;
        for (auto v : j) {
            std::cout  << v << ", ";
        }
        std::cout << std::endl;
    }

    for (auto i : alphabet) std::cout << i << " "; std::cout << std::endl;

    for (auto j : char_position) std::cout << j.first << ": " << j.second << std::endl;
}

void Tree::make_range1(std::string& str) {
    if (str.empty()) return;
    bool create = false;
    for (size_t it = 0; it < str.length(); it++) {
        if (it+1 < str.length() && str[it+1] == '-' && it+2 < str.length()) {
            unsigned char a1 = str[it];
            unsigned char an = str[it+2];
            if (a1 > an) throw std::invalid_argument("[a1 > an]");
            for (int i = a1; i <= an; i++) {
                Operands.push(std::move(make_a_node(i)));
                if (create == 1) {
                    Operands.push(std::move(get_operator('|')));
                }
                create = true;
            }
            it += 2;
        }
        else {
            Operands.push(std::move(make_a_node(str[it])));
            Operands.push(std::move(get_operator('|')));
            create = true;
        }
    }
}

void Tree::make_range2(std::string& str) {
    if (str.empty()) return;
    size_t separator = str.find(',');
    if (separator == std::string::npos) throw std::invalid_argument("не правильно задан диапазон");
    std::string x_help = str.substr(0, separator);
    std::string y_help = str.substr(separator + 1);
    int x = 0;
    int y = 0;
    if (x_help.length() != 0) x = std::stoi(x_help);
    if (y_help.length() != 0) y = std::stoi(y_help);
    std::unique_ptr<Node> top = std::move(Operands.top());
    Operands.pop();

    if (y_help.length() != 0) {
        if (x >= y) throw std::invalid_argument("x >= y");
        Operands.push(std::move(make_e_node('$')));
        for (size_t i = 1; i <= x; i++) {
            postorder(top);
            Operands.push(std::move(make_cat_node('.')));
        }
        Operands.push(std::move(make_e_node('$')));
        for (size_t i = 1; i <= y - x; i++) {
            Operands.push(std::move(make_e_node('$')));
            for (size_t j = 1; j <= i; j++) {
                postorder(top);
                Operands.push(std::move(make_cat_node('.')));
            }
            Operands.push(std::move(make_or_node('|')));
        }
        Operands.push(std::move(make_cat_node('.')));
    }
    else if (y_help.length() == 0) {
        Operands.push(std::move(make_e_node('$')));
        for (size_t i = 1; i <= x; i++) {
            postorder(top);
            Operands.push(std::move(make_cat_node('.')));
        }
        postorder(top);
        Operands.push(std::move(make_star_node('*')));
        Operands.push(std::move(make_cat_node('.')));
    } else if (y_help.length() == 0 && x_help.length() == 0) throw std::invalid_argument("ошибка");
}

void Tree::postorder(std::unique_ptr<Node> &top) {
    if (!top) return;
    postorder(top->left);
    postorder(top->right);
    char sim = top->operand;
    char ch = top->type;
    if (ch == A) Operands.push(std::move(make_a_node(sim)));
    else if (ch == EPSILON) Operands.push(std::move(make_e_node(sim)));
    else Operands.push(std::move(get_operator(sim)));
}












