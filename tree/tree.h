#pragma once
#include <string>
#include <stack>
#include <memory>
#include <set>
#include <map>
#include <unordered_set>

enum TypeNodes {
    EPSILON,
    A,
    OR,
    CAT,
    STAR,
    PLUS
};

struct Node {
    TypeNodes type;
    char operand;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;

    bool nullable;
    std::set<int> firstpos;
    std::set<int> lastpos;
};

//алгоритм Shunting Yard
class Tree {
    std::string str;
    std::stack<std::unique_ptr<Node>> Operands;
    std::stack<char> Operators;
    std::map<int, std::set<int>> followpos;
    int pos = 0;
    bool screening = false;
    std::unordered_set<char> alphabet;
    std::map<int, char> char_position;

public:
    Tree() {}
    int priority(char& c);
    std::unique_ptr<Node> make_e_node(char ch);
    std::unique_ptr<Node> make_a_node(char ch);
    std::unique_ptr<Node> make_or_node(char ch);
    std::unique_ptr<Node> make_cat_node(char ch);
    std::unique_ptr<Node> make_star_node(char ch);
    std::unique_ptr<Node> make_plus_node(char ch);
    void parsing(std::string& str);
    bool is_operator(char& ch);
    std::unique_ptr<Node> get_operator(char ch);
    std::string setToString(const std::set<int>& s);
    void writeNode(const std::unique_ptr<Node>& node, std::ostream& out);
    void exportToGraphviz(const std::string& filename, const std::unique_ptr<Node>& root);
    void ptintFP() const;
    Node* get_root() { return Operands.top().get(); }
    std::unordered_set<char>& get_alphabet() { return alphabet; }
    std::map<int, char>& get_char_position() {return char_position; }
    std::map<int, std::set<int>>& get_followpos() {return followpos; }
    int get_pos() { return pos; }
    void make_range1(std::string& str);
    void make_range2(std::string& str);
    std::unique_ptr<Node> make_reg_node(std::unique_ptr<Node>& top);
    void postorder(std::unique_ptr<Node>& top);
};