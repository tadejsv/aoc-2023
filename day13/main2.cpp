#include <algorithm>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <span>
#include <vector>

#include "utils/utils.h"

constexpr std::size_t WIDTH_32{ 31 };

auto
block_to_ints(const std::span<const std::string> block)
    -> std::tuple<std::vector<std::uint32_t>, std::vector<std::uint32_t>> {
    const std::size_t height{ block.size() };
    const std::size_t width{ block[0].size() };

    // 0 is horizontal, 1 is vertical
    std::tuple<std::vector<std::uint32_t>, std::vector<std::uint32_t>> ints;
    std::get<0>(ints).resize(height, 0);
    std::get<1>(ints).resize(width, 0);

    for (std::size_t i{ 0 }; i < height; i++) {
        for (std::size_t j{ 0 }; j < width; j++) {
            if (block[i][j] == '#') {
                std::get<0>(ints)[i] = std::get<0>(ints)[i] | (1 << (WIDTH_32 - j));
                std::get<1>(ints)[j] = std::get<1>(ints)[j] | (1 << (WIDTH_32 - i));
            }
        }
    }

    return ints;
};

// Finds reflection that has exactly one difference
auto
find_reflection(std::span<const uint32_t> block) -> unsigned long {
    for (std::size_t i{ 1 }; i < block.size(); i++) {
        int refl_difs{ 0 };
        for (std::size_t j{ 0 }; j < std::min(block.size() - i, i); j++) {
            refl_difs += std::popcount(block[i + j] ^ block[i - 1 - j]);
        }
        if (refl_difs == 1) {
            return i;
        }
    }
    return 0;
};

int
main()  // NOLINT
{
    constexpr unsigned long MUL_H{ 100 };
    const auto lines = utils::read_lines_from_file("input.txt");
    std::span lines_span{ lines };
    std::vector<std::tuple<std::size_t, std::size_t>> blocks;
    std::size_t start{ 0 };

    for (std::size_t i{ 0 }; i < lines.size(); i++) {
        std::string_view line{ lines[i] };
        if (line.empty()) {
            blocks.emplace_back(start, i - start);
            start = i + 1;
        }
    }
    blocks.emplace_back(start, lines.size() - start);

    unsigned long reflection_sum{ 0 };
    for (const auto& block_inds : blocks) {
        std::span block_span{ lines_span.subspan(std::get<0>(block_inds), std::get<1>(block_inds)) };
        const auto [block_ints_h, block_ints_v] = block_to_ints(block_span);

        reflection_sum += find_reflection(block_ints_h) * MUL_H;
        reflection_sum += find_reflection(block_ints_v);
    }

    std::cout << reflection_sum << "\n";

    return 0;
}