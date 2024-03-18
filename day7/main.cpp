#include "day7.h"
#include "utils/utils.h"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

int main() { // NOLINT
    const auto lines = utils::read_lines_from_file("input.txt");

    std::vector<std::pair<Hand, int>> hands;
    for (std::string_view line : lines) {
        const auto hand_win = utils::split_string(line, ' ');
        hands.emplace_back(Hand(hand_win.at(0)), std::stoi(hand_win.at(1)));
    }

    std::sort(hands.begin(), hands.end(),
              [](const std::pair<Hand, int> &hand1, const std::pair<Hand, int> &hand2) {
                  return compare_hands(hand1.first, hand2.first);
              });

    long winnings{0};
    for (std::size_t i{0}; i < hands.size(); ++i) {
        winnings += static_cast<long>(hands.at(i).second) * static_cast<long>(i + 1UZ);
    }

    std::cout << winnings << "\n";
    return 0;
}