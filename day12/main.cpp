#include <cstddef>
#include <iostream>
#include <queue>
#include <string>
#include <string_view>
#include <vector>

#include "utils/utils.h"

struct State {
    std::size_t current_ind;
    std::size_t group_ind;

    // Number of damaged springs already found in the group (before current position)
    int group_found;

    friend auto operator<<(std::ostream& outs, const State& state) -> std::ostream&;
};

auto
operator<<(std::ostream& outs, const State& state) -> std::ostream& {
    outs << "State: {"
         << "current_ind: " << state.current_ind << ", "
         << "group_ind: " << state.group_ind << ", "
         << "group_found: " << state.group_found << "}";
    return outs;
}

auto
check_empty(std::string_view row, const std::vector<int>& groups, const State& state, std::queue<State>& queue)
    -> int {
    // Is the previous group filled?
    if (state.group_found != 0 && state.group_found < groups[state.group_ind]) {
        return 0;
    }

    auto new_group{ state.group_ind };
    if (state.group_found != 0) {
        new_group = state.group_ind + 1;
    }

    // If this is the last character in row - did all the groups get filled?
    if (row.size() - 1 == state.current_ind) {
        if (new_group == groups.size()) {
            return 1;
        }
        return 0;
    }

    queue.emplace(state.current_ind + 1, new_group, 0);
    return 0;
};

auto
check_damaged(
    std::string_view row,
    const std::vector<int>& groups,
    const State& state,
    std::queue<State>& queue
) -> int {
    // Did we already pass the last group
    if (state.group_ind == groups.size()) {
        return 0;
    }

    // If this is the last character in row - did all the groups get filled?
    if (row.size() - 1 == state.current_ind) {
        if (state.group_found == groups[state.group_ind] - 1 && state.group_ind == groups.size() - 1) {
            return 1;
        }
        return 0;
    }

    // Still space in current group?
    if (state.group_found < groups[state.group_ind]) {
        queue.emplace(state.current_ind + 1, state.group_ind, state.group_found + 1);
    }

    return 0;
}

auto
evaluate(std::string_view row, const std::vector<int>& groups, const State& state, std::queue<State>& queue)
    -> int {
    if (row[state.current_ind] == '#') {
        return check_damaged(row, groups, state, queue);
    }

    if (row[state.current_ind] == '.') {
        return check_empty(row, groups, state, queue);
    }

    // Otherwise we have '?'
    return check_empty(row, groups, state, queue) + check_damaged(row, groups, state, queue);
};

int
main()  // NOLINT
{
    const auto lines = utils::read_lines_from_file("input.txt");

    int sum_counts{ 0 };
    for (const auto& line : lines) {
        const auto& split_res = utils::split_string(line, ' ');
        std::string_view row{ split_res[0] };
        const auto& groups_str = split_res[1];

        const auto& groups_split = utils::split_string(groups_str, ',');
        std::vector<int> groups{};
        groups.reserve(groups_split.size());
        for (const auto& group_str : groups_split) {
            groups.push_back(std::stoi(group_str));
        }

        int num_possibilities{ 0 };
        std::queue<State> queue;
        queue.emplace(0, 0, 0);

        for (; !queue.empty(); queue.pop()) {
            num_possibilities += evaluate(row, groups, queue.front(), queue);
        }

        sum_counts += num_possibilities;
        std::cout << num_possibilities << " "
                  << "\n";
    }

    std::cout << "Total: " << sum_counts << "\n";
    return 0;
}