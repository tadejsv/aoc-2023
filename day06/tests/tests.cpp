#include <catch2/catch_test_macros.hpp>

#include "day6.h"

TEST_CASE("Test num_wins") {
    REQUIRE(num_wins(7LL, 9LL) == 4LL);
}
