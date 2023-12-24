#include "day7.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <string_view>
#include <utility>

// This should be offloaded to a generator
TEST_CASE("Test hand constructor") {
    auto test_case = GENERATE(std::make_pair("AAAAA", 6), std::make_pair("TAAAA", 5),
                              std::make_pair("TAAAT", 4), std::make_pair("TAAA2", 3),
                              std::make_pair("TA1AT", 2), std::make_pair("TA17T", 1),
                              std::make_pair("JA17T", 0));

    REQUIRE(Hand(test_case.first).strength == test_case.second);
}

TEST_CASE("Test hand comparison") {
    REQUIRE(compare_hands(Hand("AAAAA"), Hand("22345")) == false);
    REQUIRE(compare_hands(Hand("AAAAA"), Hand("JJJJJ")) == false);
    REQUIRE(compare_hands(Hand("32223"), Hand("AAQQK")) == false);
    REQUIRE(compare_hands(Hand("32223"), Hand("32223")) == false);
    REQUIRE(compare_hands(Hand("62345"), Hand("AAAAA")) == true);
    REQUIRE(compare_hands(Hand("JJJJJ"), Hand("AAAAA")) == true);
    REQUIRE(compare_hands(Hand("AAQQK"), Hand("32223")) == true);
    REQUIRE(compare_hands(Hand("AJQ32"), Hand("AJQ42")) == true);
}
