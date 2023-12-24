#include "day6.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Test num_wins") {
    REQUIRE(num_wins(7LL, 9LL) == 4LL);
}
