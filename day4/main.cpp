#include "utils/utils.h"
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_set>

auto intersection(const std::unordered_set<int> &set1,
                  const std::unordered_set<int> &set2) -> std::unordered_set<int> {
    std::unordered_set<int> intersection_set;
    if (set1.size() > set2.size()) {
        for (auto element : set2) {
            if (set1.contains(element)) {
                intersection_set.insert(element);
            }
        }
    } else {
        for (auto element : set1) {
            if (set2.contains(element)) {
                intersection_set.insert(element);
            }
        }
    }

    return intersection_set;
}

struct Card {
    int name;
    std::unordered_set<int> winning_numbers;
    std::unordered_set<int> our_numbers;

    Card(std::string_view line) {
        const auto name_numbers = utils::split_string(line, ':');
        const auto name_parts = utils::split_string(name_numbers.at(0), ' ');
        name = std::stoi(name_parts.at(1));

        const auto winning_our_numbers = utils::split_string(name_numbers.at(1), '|');
        const auto our_numbers_str =
            utils::split_string(winning_our_numbers.at(0), ' ');
        const auto winning_numbers_str =
            utils::split_string(winning_our_numbers.at(1), ' ');

        our_numbers = std::unordered_set<int>();
        winning_numbers = std::unordered_set<int>();

        for (const auto &num : our_numbers_str) {
            our_numbers.insert(std::stoi(num));
        }

        for (const auto &num : winning_numbers_str) {
            winning_numbers.insert(std::stoi(num));
        }
    }

    [[nodiscard]] auto points() const -> int {
        auto lucky_numbers{intersection(our_numbers, winning_numbers)};
        if (lucky_numbers.empty()) {
            return 0;
        }

        return 1 << static_cast<int>(lucky_numbers.size() - 1);
    }
};

int main() { // NOLINT
    const auto lines = utils::read_lines_from_file("input.txt");
    int num_points{0};

    for (std::string_view line : lines) {
        Card card{line};
        num_points += card.points();
    }

    std::cout << num_points << "\n";
    return 0;
}