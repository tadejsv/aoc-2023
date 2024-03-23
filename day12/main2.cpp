// Much improved version of part1:
// * Checks for spans of # directly
// * Deduplicates paths by keeping tracks of how many different ways are there to come
//   to the same node (defined as (current_index, group_index, next_char))
// Runtime for this is 10ms, not bad.
#include "utils/utils.h"
#include <cstddef>
#include <iostream>
#include <queue>
#include <string>
#include <string_view>
#include <vector>

constexpr int EXPAND{5};

struct State {
    std::size_t current_ind;
    std::size_t group_ind;
    std::size_t next_dot;

    [[nodiscard]] auto ind(std::size_t num_groups) const -> std::size_t {
        return current_ind * (num_groups + 1) * 2 + group_ind * 2 + next_dot;
    };
};

auto check_empty(std::string_view row,
    const std::vector<int> &groups,
    const State &state,
    std::queue<State> &queue,
    std::vector<long long> &num_paths) -> long long {

    const std::size_t curr_ind{state.ind(groups.size())};

    // If this is the last character in row - did all the groups get filled?
    if (row.size() - 1 == state.current_ind) {
        if (state.group_ind == groups.size()) {
            return num_paths[curr_ind];
        }
        return 0;
    }

    std::size_t next_ind{state.current_ind + 1};
    if (row[next_ind] == '?' || row[next_ind] == '.') {
        State new_state{next_ind, state.group_ind, 1};
        const auto new_ind{new_state.ind(groups.size())};
        if (num_paths[new_ind] == 0) {
            queue.push(new_state);
        }
        num_paths[new_ind] += num_paths[curr_ind];
    }

    if (row[next_ind] == '?' || row[next_ind] == '#') {
        State new_state{next_ind, state.group_ind, 0};
        const auto new_ind{new_state.ind(groups.size())};
        if (num_paths[new_ind] == 0) {
            queue.push(new_state);
        }
        num_paths[new_ind] += num_paths[curr_ind];
    }

    return 0;
};

auto check_damaged(std::string_view row,
    const std::vector<int> &groups,
    const State &state,
    std::queue<State> &queue,
    std::vector<long long> &num_paths) -> long long {

    const std::size_t curr_ind{state.ind(groups.size())};

    // Did we already pass the last group
    if (state.group_ind == groups.size()) {
        return 0;
    }

    // Enough space in row to fill group?
    std::size_t group_size{static_cast<std::size_t>(groups[state.group_ind])};
    if (row.size() - state.current_ind < group_size) {
        return 0;
    }

    // All items in group spanned '#' or '?' ?
    for (std::size_t i{state.current_ind}; i < state.current_ind + group_size; i++) {
        if (row[i] == '.') {
            return 0;
        }
    }

    // Are we at end of row?
    if (row.size() == state.current_ind + group_size) {
        if (state.group_ind == groups.size() - 1) {
            return num_paths[curr_ind];
        }
        return 0;
    }

    // Is the next character '.' or '?' ?
    if (row[state.current_ind + group_size] == '#') {
        return 0;
    }

    State new_state{state.current_ind + group_size, state.group_ind + 1, 1};
    const auto new_ind{new_state.ind(groups.size())};
    if (num_paths[new_ind] == 0) {
        queue.push(new_state);
    }
    num_paths[new_ind] += num_paths[curr_ind];

    return 0;
}

int main() // NOLINT
{
    const auto lines = utils::read_lines_from_file("input.txt");

    long long total{0};
    for (const auto &line : lines) {
        const auto &split_res = utils::split_string(line, ' ');
        std::string_view row{split_res[0]};
        const auto &groups_str = split_res[1];

        std::string row_exp;
        for (int i = 0; i < EXPAND - 1; ++i) {
            row_exp += row;
            row_exp.push_back('?');
        }
        row_exp += row;

        const auto &groups_split = utils::split_string(groups_str, ',');
        std::vector<int> groups{};
        groups.reserve(groups_split.size());
        for (const auto &group_str : groups_split) {
            groups.push_back(std::stoi(group_str));
        }

        std::vector<int> groups_exp;
        groups_exp.reserve(EXPAND * groups.size());
        for (int i = 0; i < EXPAND; ++i) {
            groups_exp.insert(groups_exp.end(), groups.begin(), groups.end());
        }

        long long row_paths{0};
        std::queue<State> queue;
        std::vector<long long> num_paths(
            row_exp.size() * (groups_exp.size() + 1) * 2, 0);

        if (row[0] != '.') {
            queue.emplace(0, 0, 0);
            num_paths[0] = 1;
        }
        if (row[0] != '#') {
            queue.emplace(0, 0, 1);
            num_paths[1] = 1;
        }

        State state{};
        for (; !queue.empty(); queue.pop()) {
            state = queue.front();
            if (state.next_dot == 0) {
                row_paths +=
                    check_damaged(row_exp, groups_exp, state, queue, num_paths);
            } else {
                row_paths += check_empty(row_exp, groups_exp, state, queue, num_paths);
            }
        }
        total += row_paths;
    }

    std::cout << "Total: " << total << "\n";
    return 0;
}