#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include "doctest.h"
#include "OrgChart.hpp"

using namespace std;
using namespace ariel;


TEST_CASE("Check all traversals order on normal case"){

    /*
                   A
                 / | \    
                /  |   \
              B    C     D
             / \        /  \
            E    F     O    P
            |        / | \
            G       R  S  T
            |
            H
    */

    OrgChart alphabet;
    CHECK_NOTHROW(alphabet.add_root("A"));
    CHECK_NOTHROW(alphabet.add_sub("A", "B"));
    CHECK_NOTHROW(alphabet.add_sub("A", "C"));
    CHECK_NOTHROW(alphabet.add_sub("A", "D"));
    CHECK_NOTHROW(alphabet.add_sub("D", "O"));
    CHECK_NOTHROW(alphabet.add_sub("O", "R"));
    CHECK_NOTHROW(alphabet.add_sub("O", "S"));
    CHECK_NOTHROW(alphabet.add_sub("O", "T"));
    CHECK_NOTHROW(alphabet.add_sub("D", "P"));
    CHECK_NOTHROW(alphabet.add_sub("B", "E"));
    CHECK_NOTHROW(alphabet.add_sub("B", "F"));
    CHECK_NOTHROW(alphabet.add_sub("E", "G"));
    CHECK_NOTHROW(alphabet.add_sub("G", "H"));


    cout << alphabet << endl;

    /*
             +- A
                +- B
                |  +- E
                |  |  +- G
                |  |     +- H
                |  +- F
                +- C
                +- D
                    +- O
                    |  +- R
                    |  +- S
                    |  +- T
                    +- P


    */
   
 
    vector<string> vector_lev = {"A","B","C","D","E","F","O","P","G","R","S","T","H"};
    auto iter1 = alphabet.begin_level_order();
    uint i = 0;
    while (iter1 != alphabet.end_level_order())
    {
        CHECK((*iter1) == vector_lev[i]);
        i++;
        ++iter1;
    }

    vector<string> vector_rev = {"H","G","R","S","T","E","F","O","P","B","C","D","A"};
    auto iter2 = alphabet.begin_reverse_order();
    uint j = 0;
    while (iter2 != alphabet.end_reverse_order())
    {
        CHECK((*iter2) == vector_rev[j]);
        j++;
        ++iter2;
    }

    vector<string> vector_pre = {"A","B","E","G","H","F","C","D","O","R","S","T","P"};
    auto iter3 = alphabet.begin_preorder();
    uint z = 0;
    while (iter3 != alphabet.end_preorder())
    {
        CHECK((*iter3) == vector_pre[z]);
        z++;
        ++iter3;
    }


}


TEST_CASE("Check all traversals order on extrem case")
{

    /*
        "one"
            \
            "two"
                \
                "three"
                    \
                    "four"
                        \
                        "five"
                            \
                            "six"
                                \
                                "seven"
                                    \
                                    "eight"
                                        \
                                        "nine"
                                            \
                                            "ten"

    */

    OrgChart numbers;
    CHECK_NOTHROW(numbers.add_root("one"));
    CHECK_NOTHROW(numbers.add_sub("one", "two"));
    CHECK_NOTHROW(numbers.add_sub("two", "three"));
    CHECK_NOTHROW(numbers.add_sub("three", "four"));
    CHECK_NOTHROW(numbers.add_sub("four", "five"));
    CHECK_NOTHROW(numbers.add_sub("five", "six"));
    CHECK_NOTHROW(numbers.add_sub("six", "seven"));
    CHECK_NOTHROW(numbers.add_sub("seven", "eight"));
    CHECK_NOTHROW(numbers.add_sub("eight", "nine"));
    CHECK_NOTHROW(numbers.add_sub("nine", "ten"));

   

    // Check for reverse order traversal
    vector<string> vector_rev = {"ten", "nine", "eight", "seven", "six", "five", "four", "three", "two", "one"};
    auto it_rev = numbers.begin_reverse_order();
    uint i = 0;
    while (it_rev != numbers.end_reverse_order())
    {
        CHECK((*it_rev) == vector_rev[i]);
        i++;
        ++it_rev;
    }

    // Check for level order traversal
    vector<string> vector_lev = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten"};
    auto it_lev = numbers.begin_level_order();
    i = 0;
    while (it_lev != numbers.end_level_order())
    {
        CHECK((*it_lev) == vector_lev[i]);
        i++;
        it_lev++;
    }

    // Check for preorder traversal
     auto itPost = numbers.begin_preorder();
    i = 0;
    while (itPost != numbers.end_preorder())
    {
        CHECK((*itPost) == vector_lev[i]);
        i++;
        itPost++;
    }
   
   cout << numbers << endl;

    /*
        +- one
            +- two
                +- three
                    +- four
                        +- five
                            +- six
                                +- seven
                                    +- eight
                                        +- nine
                                            +- ten

    */
}


TEST_CASE("simple iterators") {

    SUBCASE("== / !=") {

        OrgChart School;
        School.add_root("Director").add_sub("Director", "Secretary");
        auto it1 = School.begin();
        auto it2 = School.begin_level_order();
        auto it3 = School.begin_reverse_order();
        auto it4 = School.begin_preorder();

        CHECK(it1 == it2);
        CHECK(it2 == it4);
        CHECK(it3 != it1);
        CHECK(it4 != it3);

    }

    SUBCASE("++") {

        OrgChart School;
        School.add_root("Director").add_sub("Director", "Secretary").add_sub("Director", "Supervisor");
        auto it1 = School.begin();
        auto it2 = School.begin_level_order();
        auto it3 = School.begin_reverse_order();
        auto it4 = School.begin_preorder();

        CHECK_EQ(it1, it4);
        CHECK_EQ(it1, it2);
        CHECK_NOTHROW(it1++);
        CHECK_NE(it1, it4);
        CHECK_NOTHROW(it4++);
        CHECK_EQ(it1, it4);
        CHECK_NOTHROW(it3++);
        CHECK_NE(it3, it2);


    }
}
