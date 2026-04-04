#include "tree/tree.h"
#include "iostream"
#include "minDFA.h"
#include "regex.h"

int main() {
    // Tree t;
    // DFABuilder DFA;
    //DFA a;
    try {
        std::cout << "enter string: ";
        std::string str;
        std::cin >> str;
        Regex r(str);
        std::string regex;
        std::cout << "enter regex: ";
        std::cin >> regex;
        bool re = r.match(regex);
        if (re) {
            std::cout << "OK" << std::endl;
        } else {
            std::cout << "NOT OK" << std::endl;
        }


        // std::cout << "enter string: ";
        // std::string str2;
        // std::cin >> str2;
        // r.compile(str2);
        // std::string regex2;
        // std::cout << "enter regex: ";
        // std::cin >> regex2;
        // bool re2 = r.match(regex2);
        // if (re2) {
        //     std::cout << "OK" << std::endl;
        // } else {
        //     std::cout << "NOT OK" << std::endl;
        // }


    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }


    return 0;
}
