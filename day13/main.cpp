#include "utils/utils.h"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <span>
#include <vector>

constexpr std::size_t WIDTH_32{31};

auto block_to_ints(const std::span<const std::string> &block, bool horizontal)
    -> std::vector<std::uint32_t> {
    const std::size_t height{block.size()};
    const std::size_t width{block[0].size()};

    std::vector<std::uint32_t> ints;
    ints.reserve(horizontal ? height : width);

    if (horizontal) {
        for (std::size_t i{0}; i < height; i++) {
            std::uint32_t block_int{0};
            for (std::size_t j{0}; j < width; j++) {
                if (block[i][j] == '#') {
                    block_int = block_int | (1 << (WIDTH_32 - j));
                }
            }
            ints.push_back(block_int);
        }
    } else {
        for (std::size_t i{0}; i < width; i++) {
            std::uint32_t block_int{0};
            for (std::size_t j{0}; j < height; j++) {
                if (block[j][i] == '#') {
                    block_int = block_int | (1 << (WIDTH_32 - j));
                }
            }
            ints.push_back(block_int);
        }
    }

    return ints;
};

auto find_reflection(std::span<const uint32_t> block) -> unsigned long {
    for (std::size_t i{1}; i < block.size(); i++) {
        if (block[i] == block[i - 1]) {
            bool all_reflect{true};
            for (std::size_t j{1}; j < std::min(block.size() - i, i); j++) {
                if (block[i + j] != block[i - 1 - j]) {
                    all_reflect = false;
                    break;
                }
            }
            if (all_reflect) {
                return i;
            }
        }
    }

    return 0;
};

int main() // NOLINT
{
    constexpr unsigned long MUL_H{100};
    const auto lines = utils::read_lines_from_file("input.txt");
    std::span lines_span{lines};
    std::vector<std::tuple<std::size_t, std::size_t>> blocks;
    std::size_t prev_start{0};

    for (std::size_t i{0}; i < lines.size(); i++) {
        std::string_view line{lines[i]};
        if (line.empty()) {
            blocks.emplace_back(prev_start, i - prev_start);
            prev_start = i + 1;
        }
    }
    blocks.emplace_back(prev_start, lines.size() - prev_start);

    unsigned long reflection_sum{0};
    for (auto const &block_inds : blocks) {
        std::span block_span{
            lines_span.subspan(std::get<0>(block_inds), std::get<1>(block_inds))};
        auto const block_ints_h = block_to_ints(block_span, true);
        auto const block_ints_v = block_to_ints(block_span, false);

        reflection_sum += find_reflection(block_ints_h) * MUL_H;
        reflection_sum += find_reflection(block_ints_v);
    }

    std::cout << reflection_sum << "\n";

    return 0;
}