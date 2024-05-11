#include "utils/utils.h"
#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

struct Box {
    std::unordered_map<std::string, int> map;
    std::vector<std::string> order;

    auto insert(const std::string &label, int focal_len) -> void {
        if (!map.contains(label)) {
            order.push_back(label);
        }

        map[label] = focal_len;
    };

    auto remove(const std::string &label) -> void {
        if (map.contains(label)) {
            map.erase(label);
            std::erase(order, label);
        }
    };

    [[nodiscard]] auto number() const -> long long {
        long long sum{0};
        for (std::size_t i{0}; i < order.size(); ++i) {
            sum += static_cast<long long>(i + 1) *
                   static_cast<long long>(map.at(order[i]));
        }

        return sum;
    };
};

auto hash(std::string_view input) -> int {
    int current_hash{0};
    for (const auto &chr : input) {
        current_hash = ((current_hash + chr) * 17) % 256; // NOLINT
    }

    return current_hash;
};

int main() // NOLINT
{
    const auto lines = utils::read_lines_from_file("input.txt");

    std::array<Box, 256> boxes; // NOLINT
    for (const auto &line : lines) {
        const auto parts{utils::split_string(line, ',')};
        for (const auto &part : parts) {
            if (part[part.size() - 1] == '-') {
                const auto &label{part.substr(0, part.size() - 1)};
                const auto box_num{hash(label)};
                boxes.at(static_cast<std::size_t>(box_num)).remove(label);
            } else {
                const auto &label{part.substr(0, part.size() - 2)};
                const auto box_num{hash(label)};
                int focal_length{part[part.size() - 1] - '0'};
                boxes.at(static_cast<std::size_t>(box_num)).insert(label, focal_length);
            }
        }
    }

    long long sum{0};
    for (std::size_t i{0}; i < boxes.size(); ++i) {
        sum += static_cast<long long>(i + 1) * boxes.at(i).number();
    }

    std::cout << sum << "\n";

    return 0;
}