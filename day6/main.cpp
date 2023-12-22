#include "utils/utils.h"
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

auto num_wins(const long total_time, const long record) -> long {
    auto const square_coeff = total_time * total_time - 4 * record;
    if (square_coeff < 0L) {
        return 0L;
    }

    // Subtracting epsilon to make  sure we always get more than record
    auto const root_coeff{std::sqrt(square_coeff) - 0.00001};
    auto const middle{static_cast<double>(total_time) / 2.0};

    auto const start{middle - root_coeff / 2.0};
    auto const end{middle + root_coeff / 2.0};

    auto const starti{static_cast<long>(std::ceil(start))};
    auto const endi{static_cast<long>(std::floor(end))};

    return endi - starti + 1L;
}

int main() { // NOLINT
    const auto lines = utils::read_lines_from_file("input.txt");

    const auto times = utils::split_string(utils::split_string(lines[0], ':')[1], ' ');
    const auto records =
        utils::split_string(utils::split_string(lines[1], ':')[1], ' ');

    long wins_prod{1L};
    for (std::size_t i{0}; i < times.size(); ++i) {
        wins_prod *= num_wins(std::stol(times[i]), std::stol(records[i]));
    }
    std::cout << wins_prod;
    return 0;
}