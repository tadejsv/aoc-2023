#include "utils/utils.h"
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

auto num_wins(const long long total_time, const long long record) -> long long {
    auto const square_coeff = total_time * total_time - 4 * record;
    if (square_coeff < 0L) {
        return 0L;
    }

    // Subtracting epsilon to make  sure we always get more than record
    auto const root_coeff{std::sqrt(square_coeff) - 0.00001};
    auto const middle{static_cast<double>(total_time) / 2.0};

    auto const start{middle - root_coeff / 2.0};
    auto const end{middle + root_coeff / 2.0};

    auto const starti{static_cast<long long>(std::ceil(start))};
    auto const endi{static_cast<long long>(std::floor(end))};

    return endi - starti + 1L;
}

int main() { // NOLINT
    const auto lines = utils::read_lines_from_file("input.txt");

    const auto time_parts =
        utils::split_string(utils::split_string(lines[0], ':')[1], ' ');
    const auto record_parts =
        utils::split_string(utils::split_string(lines[1], ':')[1], ' ');

    std::string time_string =
        std::accumulate(time_parts.begin(), time_parts.end(), std::string{});

    std::string record_string =
        std::accumulate(record_parts.begin(), record_parts.end(), std::string{});

    std::cout << num_wins(std::stoll(time_string), std::stoll(record_string));
    return 0;
}