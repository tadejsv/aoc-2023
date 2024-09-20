#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "utils/utils.h"

#include "day6.h"

int
main() {  // NOLINT
    const auto lines = utils::read_lines_from_file("input.txt");

    const auto times = utils::split_string(utils::split_string(lines[0], ':')[1], ' ');
    const auto records = utils::split_string(utils::split_string(lines[1], ':')[1], ' ');

    long wins_prod{ 1LL };
    for (std::size_t i{ 0 }; i < times.size(); ++i) {
        wins_prod *= num_wins(std::stoll(times[i]), std::stoll(records[i]));
    }
    std::cout << wins_prod;
    return 0;
}