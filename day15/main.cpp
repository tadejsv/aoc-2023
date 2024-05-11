#include "utils/utils.h"
#include <iostream>
#include <string_view>

auto hash(std::string_view input) -> int {
    int current_hash{0};
    for (const auto &chr : input) {
        current_hash = ((current_hash + chr) *
                        17); // NOLINT (cppcoreguidelines-avoid-magic-numbers)
        current_hash %= 256; // NOLINT (cppcoreguidelines-avoid-magic-numbers)
    }

    return current_hash;
};

int main() // NOLINT
{
    const auto lines = utils::read_lines_from_file("input.txt");

    long long hash_sum{0};
    for (const auto &line : lines) {
        const auto parts{utils::split_string(line, ',')};
        for (const auto &part : parts) {
            hash_sum += hash(part);
        }
    }

    std::cout << hash_sum << "\n";

    return 0;
}