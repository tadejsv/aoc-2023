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

    CAPTURE(test_case.first, test_case.second);
    REQUIRE(Hand(test_case.first).strength == test_case.second);
}

TEST_CASE("Test hand comparison") {
    std::tuple<const char *, const char *, bool> test_case =
        GENERATE(std::make_tuple("AAAAA", "22345", false),
                 std::make_tuple("AAAAA", "JJJJJ", false),
                 std::make_tuple("32223", "AAQQK", false),
                 std::make_tuple("32223", "32223", false),
                 std::make_tuple("62345", "AAAAA", true),
                 std::make_tuple("JJJJJ", "AAAAA", true),
                 std::make_tuple("AAQQK", "32223", true),
                 std::make_tuple("AJQ32", "AJQ42", true));

    CAPTURE(std::get<0>(test_case), std::get<1>(test_case), std::get<2>(test_case));
    REQUIRE(compare_hands(Hand(std::get<0>(test_case)), Hand(std::get<1>(test_case))) ==
            std::get<2>(test_case));
}
