#include "utils/utils.h"
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

auto predict_next_number(const std::span<long> input) -> long {
    if (std::ranges::all_of(input, [](long num) { return num == 0L; })) {
        return 0L;
    }

    std::vector<long> diff(input.size() - 1);
    for (std::size_t i{1}; i < input.size(); ++i) {
        diff[i - 1] = input[i] - input[i - 1];
    }
    auto pred_diff{predict_next_number(diff)};
    return input.back() + pred_diff;
};

int main() { // NOLINT
    const auto lines = utils::read_lines_from_file("input.txt");
    long sum{0};
    for (const auto &line : lines) {
        const auto numbersStr = utils::split_string(line, ' ');
        std::vector<long> numbers{};
        numbers.reserve(numbersStr.size());
        for (auto const &num : numbersStr) {
            numbers.push_back(std::stol(num));
        }

        sum += predict_next_number(numbers);
    }

    std::cout << sum;
}