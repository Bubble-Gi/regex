#include "tree/tree.h"
#include "iostream"
#include "minDFA.h"
#include "regex.h"

int main() {
    // Tree t;
    // DFABuilder DFA;
    //DFA a;
    try {
        //std::cout << "enter string: ";
        std::string str = "a{3,10}.a.(a|x)*.[a-zA-Z]*";
        //std::cin >> str;
        Regex r(str);
        r.print();
        std::string regex = "aaaaaaaaaaaaxaxaxaxaxSupercalifragilisticexpialidocious";
        //std::cout << "enter regex: ";
        //std::cin >> regex;
        bool re = r.match(regex);
        if (re) {
            std::cout << "OK" << std::endl;
        } else {
            std::cout << "NOT OK" << std::endl;
        }
         std::string rec = r.recoveryy();
         std::cout << rec << std::endl;
         Regex r2(rec);
         bool eq = r.equile(r2);
         std::string inv = r.inverse(str);
        Regex r3(inv);
         std::cout << inv << std::endl;
         std::string regex1 = "suoicodilaipxecitsiligarfilacrepuSxaxaxaxaxaaaaaaaaaaaa";
         bool i = r3.match(regex1);
        if (re) {
            std::cout << "OK" << std::endl;
        } else {
            std::cout << "NOT OK" << std::endl;
        }
        if (i) {
            std::cout << "OK" << std::endl;
        } else {
            std::cout << "NOT OK" << std::endl;
        }
         if (eq) {
             std::cout << "eq" << std::endl;
         } else {
             std::cout << "NOT eq" << std::endl;
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
