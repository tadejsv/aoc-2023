#include "utils/utils.h"
#include <cstddef>
#include <iostream>
#include <vector>

int main() // NOLINT
{
    const auto lines = utils::read_lines_from_file("input.txt");
    const auto num_rows{lines.size()};

    std::size_t total_weight{0};
    for (std::size_t i{0}; i < lines[0].size(); ++i) {
        std::size_t current_offset{0};
        for (std::size_t j{0}; j < num_rows; ++j) {
            if (lines[j][i] == 'O') {
                total_weight += num_rows - current_offset;
                current_offset += 1;
            } else if (lines[j][i] == '#') {
                current_offset = j + 1;
            }
        }
    }

    std::cout << total_weight << "\n";

    return 0;
}