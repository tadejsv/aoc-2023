#include "day6.h"
#include "utils/utils.h"
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

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