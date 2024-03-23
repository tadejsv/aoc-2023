#include "utils/utils.h"
#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

struct State {
    std::string_view row;
    std::vector<int> *groups;
    std::size_t current_ind;
    std::size_t group_ind;
    int group_prev;
};

int main() { // NOLINT
    const auto lines = utils::read_lines_from_file("test_input.txt");

    for (const auto &line : lines) {
        const auto &split_res = utils::split_string(line, ' ');
        const auto &row = split_res[0];
        const auto &groups_str = split_res[1];

        const auto &groups_split = utils::split_string(groups_str, ',');
        std::vector<int> groups{};
        groups.reserve(groups_split.size());
        for (const auto &group_str : groups_split) {
            std::cout << group_str << "\n";
            groups.push_back(std::stoi(group_str));
        }
        std::cout << groups[0] << groups[1] << groups[2] << "\n";
        std::cout << line << "\n";
    }

    return 0;
}