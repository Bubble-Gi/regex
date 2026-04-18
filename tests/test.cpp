#include <catch2/catch_all.hpp>
#include "tree/tree.h"
#include "regex/regex.h"
#include "DFA/minDFA.h"

//добавить пустой язык
TEST_CASE("cat_string") {
    SECTION("ab") {
        Tree t;
        std::string s = "ab";
        std::string r = t.cat_string(s);
        REQUIRE(r == "(a.b)");
    }

    SECTION("(ab)(cd)") {
        Tree t;
        std::string s = "(ab)(cd)";
        std::string r = t.cat_string(s);
        REQUIRE(r == "((a.b).(c.d))");
    }

    SECTION("a[a-z](a|b){3,10}") {
        Tree t;
        std::string s = "a[a-z](a|b){3,10}";
        std::string r = t.cat_string(s);
        REQUIRE(r == "(a.[a-z].(a|b){3,10})");
    }

    SECTION("a*b*c(a*b)") {
        Tree t;
        std::string s = "a*b*c(a*b)";
        std::string r = t.cat_string(s);
        REQUIRE(r == "(a*.b*.c.(a*.b))");
    }

    SECTION("a%%b") {
        Tree t;
        std::string s = "a%%b";
        std::string r = t.cat_string(s);
        REQUIRE(r == "(a.%%.b)");
    }

    SECTION("a%a%{{3,5}") {
        Tree t;
        std::string s = "a%a%{{3,5}";
        std::string r = t.cat_string(s);
        REQUIRE(r == "(a.%a.%{{3,5})");
    }

    SECTION("([a-zA-Z]+)( +-[a-zA-Z]+)*( +[a-zA-Z]+)* *") {
        Tree t;
        std::string s = "([a-zA-Z]+)( +-[a-zA-Z]+)*( +[a-zA-Z]+)* *";
        std::string r = t.cat_string(s);
        REQUIRE(r == "(([a-zA-Z]+).( +.-.[a-zA-Z]+)*.( +.[a-zA-Z]+)*. *)");
    }

    SECTION("") {
        Tree t;
        std::string s = "";
        std::string r = t.cat_string(s);
        REQUIRE(r == "()");
    }

    SECTION("a...b") {
        Tree t;
        std::string s = "a...b";
        std::string r = t.cat_string(s);
        REQUIRE(r == "(a.b)");
    }

    SECTION("") {
        Tree t;
        std::string s = "";
        std::string r = t.cat_string(s);
        REQUIRE(r == "()");
    }
}

TEST_CASE("match") {
    SECTION("base") {
        Regex x("ab|c");
        REQUIRE(x.match("ab"));
        REQUIRE(x.match("c"));
        REQUIRE(!(x.match("abc")));
    }

    SECTION("id") {
        Regex x("L(L|D)*");
        REQUIRE(x.match("LDDDLDLD"));
        REQUIRE(!x.match("DLLDLDLD"));
        REQUIRE(!x.match("D"));
        REQUIRE(x.match("L"));
    }

    SECTION("range1") {
        Regex x("[a-zA-Z]*");
        REQUIRE(x.match("abracadabra"));
        REQUIRE(!x.match("-"));
        REQUIRE(x.match("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM"));
    }

    SECTION("range1.2") {
        Regex x("[a-zA-Z]+(a|b)%?*");
        REQUIRE(x.match("qwertyuiopqa?????"));
        REQUIRE(!x.match("a"));
        REQUIRE(x.match("qwertyuiopqa"));
    }

    SECTION("range1.3") {
        Tree t;
        std::string s = "[z-aZ-A]";
        REQUIRE_THROWS_AS(t.parsing(s, true), std::invalid_argument);
    }

    SECTION("range1.4") {
        Regex x("[abcdefg]+");
        REQUIRE(x.match("gfedcba"));
    }

    SECTION("range2") {
        Regex x("a{3,6}");
        REQUIRE(x.match("aaa"));
        REQUIRE(x.match("aaaaaa"));
        REQUIRE(!x.match("aaaaaaa"));
        REQUIRE(!x.match("aa"));
        REQUIRE(!x.match(""));
    }

    SECTION("range2.1") {
        Regex x("([a-z]){2,4}");
        REQUIRE(x.match("aa"));
        REQUIRE(x.match("ssss"));
        REQUIRE(x.match("jjj"));
        REQUIRE(!x.match("q"));
        REQUIRE(!x.match("lllll"));
    }

    SECTION("range2.2") {
        Regex x("a{,4}");
        REQUIRE(x.match("aa"));
        REQUIRE(x.match("aaaa"));
        REQUIRE(!x.match("aaaaaa"));
        REQUIRE(x.match(""));
    }

    SECTION("range2.3") {
        Regex x("a{3,}");
        REQUIRE(!x.match("aa"));
        REQUIRE(x.match("aaaaaaa"));
        REQUIRE(!x.match("a"));
    }

    SECTION("range2.4") {
        Tree t;
        std::string s = "a{2,1}";
        REQUIRE_THROWS_AS(t.parsing(s, true), std::invalid_argument);
    }

    SECTION("range2.5") {
        Tree t;
        std::string s = "a{21}";
        REQUIRE_THROWS_AS(t.parsing(s, true), std::invalid_argument);
    }

    SECTION("range2.6") {
        Tree t;
        std::string s = "a{0,0}";
        REQUIRE_THROWS_AS(t.parsing(s, true), std::invalid_argument);
    }

    SECTION("+") {
        Regex x("[a-z]+[A-Z]+");
        REQUIRE(x.match("qwertyuiopasdfghjklzxcvbnmQWERTYUIOOPASDFGHJKLZXCVBNM"));
        REQUIRE(!x.match("Aa"));
        REQUIRE(!x.match(""));
    }

    SECTION("...") {
        Regex x("a...b");
        REQUIRE(x.match("ab"));
    }

    SECTION("e node") {
        Regex x("a$qwerty$s");
        REQUIRE(x.match("aqwertys"));
    }

    SECTION("$") {
        Regex x("$");
        REQUIRE(x.match(""));
    }

    SECTION("") {
        Regex x("");
        REQUIRE(x.match(""));
    }

    SECTION("/1") {
        Regex x("a(bc)*/(dy)+");
        REQUIRE(x.match("abcbcbcbcdy"));
    }

    SECTION("/2") {
        Regex x("a(bc)*/(dy)+");
        REQUIRE(x.match("ady"));
    }

    SECTION("/3") {
        Regex x("a(bc)*/(dy)+");
        REQUIRE(!x.match("abcbdy"));
    }

    SECTION("/4") {
        Regex x("a+/a+");
        REQUIRE(x.match("aaaaa"));
    }

    SECTION("/5") {
        Regex x("a+/a+");
        REQUIRE(!x.match("a"));
    }

    SECTION("/5") {
        Regex x("a+/a+");
        REQUIRE(!x.match("aabb"));
    }

    SECTION("THROW /") {
        std::string str = "a(bc)*/(dy)+/A{3,5}";
        REQUIRE_THROWS_AS(Regex(str), std::invalid_argument);
    }
}

TEST_CASE("recovery") {
    //+
    //кривые тесты
    SECTION("recovery true") {
        Regex x("L.(L|D)*");
        std::string s = x.recoveryy();
        Regex y(s);
        bool eq = x.equile(y);
        REQUIRE(eq);
    }

    SECTION("recovery empty") {
        Regex x("");
        std::string s = x.recoveryy();
        Regex y(s);
        bool eq = x.equile(y);
        REQUIRE(eq);
    }

    SECTION("recovery false") {
        Regex x("L.(L|D)*");
        std::string s = x.recoveryy();
        s.append("L+");
        Regex y(s);
        bool eq = x.equile(y);
        REQUIRE(!eq);
    }

    SECTION("recovery /") {
        Regex x("a(bc)*/(dy)+");
        std::string s;
        REQUIRE_THROWS_AS(s = x.recoveryy(), std::runtime_error);
    }


}

TEST_CASE("inv") {
    SECTION("inv1") {
        std::string str = "a{3,10}.a.(a|x)*.[a-zA-Z]*";
        Regex r(str);
        std::string inv = r.inverse(str);
        Regex r2(inv);
        std::string regex1 = "suoicodilaipxecitsiligarfilacrepuSxaxaxaxaxaaaaaaaaaaaa";
        REQUIRE(r2.match(regex1));
    }

    SECTION("inv2") {
        std::string str = "a{3,10}.a.(a|x)*.[a-zA-Q]*";
        Regex r(str);
        std::string inv = r.inverse(str);
        inv.append("S+");
        Regex r2(inv);
        std::string regex1 = "suoicodilaipxecitsiligarfilacrepuSxaxaxaxaxaaaaaaaaaaaa";
        REQUIRE(!r2.match(regex1));
    }

    SECTION("inv """) {
        std::string str = "";
        Regex r(str);
        std::string inv = r.inverse(str);
        Regex r2(inv);
        std::string regex1 = "";
        REQUIRE(r2.match(regex1));
    }

    SECTION("inv3") {
        std::string str = "a";
        Regex r(str);
        std::string inv = r.inverse(str);
        Regex r2(inv);
        std::string regex1 = "a";
        REQUIRE(r2.match(regex1));
    }

    SECTION("inv4") {
        std::string str = "(abc)*";
        Regex r(str);
        std::string inv = r.inverse(str);
        Regex r2(inv);
        std::string regex1 = "cba";
        REQUIRE(r2.match(regex1));
    }

    SECTION("inv1") {
        std::string str = "(abc){3,10}";
        Regex r(str);
        std::string inv = r.inverse(str);
        Regex r2(inv);
        std::string regex1 = "cbacbacbacba";
        REQUIRE(r2.match(regex1));
    }

    SECTION("inv1") {
        std::string str = "(abc){3,10}";
        Regex r(str);
        std::string inv = r.inverse(str);
        Regex r2(inv);
        std::string regex1 = "cbacba";
        REQUIRE(!r2.match(regex1));
    }

    SECTION("inv1") {
        std::string str = "(abc){3,10}";
        Regex r(str);
        std::string inv = r.inverse(str);
        Regex r2(inv);
        std::string regex1 = "cbacbacbacbacbacbacbacbacbacbacbacba";
        REQUIRE(!r2.match(regex1));
    }

}

TEST_CASE("inter") {
    //пустота +
    SECTION("inter1") {
        Regex r1("a(a|b)*");
        Regex r2("a(a|b)*c");
        REQUIRE(r1.equile(r1));
        REQUIRE(r2.equile(r2));
        Regex r3 = r1.intersection(r2);
        REQUIRE(!r3.match(""));
        REQUIRE(!r3.match("a"));
        REQUIRE(!r3.match("ac"));
    }

    SECTION("inter2") {
        Regex r1("a{3,10}.a.(a|x)*.[a-zA-Z]*");
        Regex r2("a{3,8}.a.(a|b)*.[a-zA-W]*");
        REQUIRE(r1.equile(r1));
        REQUIRE(r2.equile(r2));
        Regex r3 = r1.intersection(r2);
        REQUIRE(r3.match("aaaaaa"));
        REQUIRE(r3.match("aaaaaaaaaaaW"));
        REQUIRE(r3.match("aaaaaaaaaaaabcd"));
        REQUIRE(!r3.match("aaaaaaZZZ"));
    }

    SECTION("inter3") {
        Regex r1("");
        Regex r2("");
        REQUIRE(r1.equile(r1));
        REQUIRE(r2.equile(r2));
        Regex r3 = r1.intersection(r2);
        REQUIRE(r3.match(""));
    }

    SECTION("inter4") {
        Regex r1("");
        Regex r2("a*(a|b)*");
        REQUIRE(r1.equile(r1));
        REQUIRE(r2.equile(r2));
        Regex r3 = r1.intersection(r2);
        REQUIRE(r3.match(""));
        REQUIRE(!r3.match("a"));
    }

    SECTION("inter5") {
        Regex r1("a{3,10}.a.(a|x)*.[a-zA-Z]*");
        Regex r2("");
        REQUIRE(r1.equile(r1));
        REQUIRE(r2.equile(r2));
        Regex r3 = r1.intersection(r2);
        REQUIRE(!r3.match(""));
        REQUIRE(!r3.match("aaaaaaaaaaaW"));
        REQUIRE(!r3.match("aaaaaaaaaaaabcd"));
        REQUIRE(!r3.match("aaaaaaZZZ"));
    }
}

TEST_CASE("MINIMIZATION") {
    SECTION("MINIMIZATION1") {
        DFABuilder b;
        b.make_tree("(a|b)*.a.(a|b|c)*|(b|c)*.b.(a|b|c)*");
        b.makeDFA();
        REQUIRE(b.get_all_states().size() == 10);
        REQUIRE(b.get_finaly_states_before_min().size() == 7);
        b.minimization_by_Hopcroft();
        REQUIRE(b.get_all_states().size() == 4);
        REQUIRE(b.get_finaly_states().size() == 1);
    }

    SECTION("MINIMIZATION2") {
        DFABuilder b;
        b.make_tree("");
        b.makeDFA();
        REQUIRE(b.get_all_states().size() == 1);
        REQUIRE(b.get_finaly_states_before_min().size() == 1);
        b.minimization_by_Hopcroft();
        REQUIRE(b.get_all_states().size() == 1);
        REQUIRE(b.get_finaly_states().size() == 1);
    }

    SECTION("MINIMIZATION3") {
        DFABuilder b;
        b.make_tree("L.(L|D)*");
        b.makeDFA();
        REQUIRE(b.get_all_states().size() == 3);
        REQUIRE(b.get_finaly_states_before_min().size() == 1);
        b.minimization_by_Hopcroft();
        REQUIRE(b.get_all_states().size() == 3);
        REQUIRE(b.get_finaly_states().size() == 1);
    }

    SECTION("MINIMIZATION4") {
        DFABuilder b;
        b.make_tree("L");
        b.makeDFA();
        REQUIRE(b.get_all_states().size() == 3);
        REQUIRE(b.get_finaly_states_before_min().size() == 1);
        b.minimization_by_Hopcroft();
        REQUIRE(b.get_all_states().size() == 3);
        REQUIRE(b.get_finaly_states().size() == 1);
    }

    SECTION("MINIMIZATION5") {
        DFABuilder b;
        b.make_tree("(L|D)C");
        b.makeDFA();
        REQUIRE(b.get_all_states().size() == 4);
        REQUIRE(b.get_finaly_states_before_min().size() == 1);
        b.minimization_by_Hopcroft();
        REQUIRE(b.get_all_states().size() == 4);
        REQUIRE(b.get_finaly_states().size() == 1);
    }

    SECTION("MINIMIZATION6") {
        DFABuilder b;
        b.make_tree("(L|D)*");
        b.makeDFA();
        REQUIRE(b.get_all_states().size() == 1);
        REQUIRE(b.get_finaly_states_before_min().size() == 1);
        b.minimization_by_Hopcroft();
        REQUIRE(b.get_all_states().size() == 1);
        REQUIRE(b.get_finaly_states().size() == 1);
    }


}