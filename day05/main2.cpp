#include <algorithm>
#include <array>
#include <iostream>
#include <limits>
#include <string>
#include <string_view>
#include <vector>

#include "utils/utils.h"

struct RangeIndex {
    long long key_start;
    long long val_start;
    long long length;
};

auto
compare_ranges(const RangeIndex& range_1, const RangeIndex& range_2) -> bool {
    return (range_1.key_start < range_2.key_start);
}

class RangeMap {
private:

    std::vector<RangeIndex> ranges;

public:

    auto add_range(const RangeIndex& range_index) -> void {
        ranges.push_back(range_index);
    };

    [[nodiscard]] auto find(const long long key) const -> long long {
        for (const auto& index : ranges) {
            if (key >= index.key_start && key < index.key_start + index.length) {
                return index.val_start + (key - index.key_start);
            }
        }

        return key;
    };

    auto sort() -> void {
        std::sort(ranges.begin(), ranges.end(), compare_ranges);
    }

    auto intersect_ranges(const std::vector<std::array<long long, 2>>& key_ranges)
        -> std::vector<std::array<long long, 2>> {
        std::vector<std::array<long long, 2>> val_ranges;

        for (const auto& key_range : key_ranges) {
            const auto start{ key_range.at(0) };
            const auto end{ key_range.at(1) };  // Inclusive
            auto current_pos{ start };

            for (const auto& range : ranges) {
                const auto range_end = range.key_start + range.length - 1LL;

                // Check if range ends before start of key range
                if (range_end < start) {
                    continue;
                }

                // Check if range starts after end of key range
                if (range.key_start > end) {
                    break;
                }

                // Fill the gap between the current_pos and start of range with identity
                if (range.key_start > current_pos) {
                    val_ranges.push_back(std::array<long long, 2>{ current_pos,
                                                                   range.key_start - 1LL });
                    current_pos = range.key_start;
                }

                const auto key_end = std::min(end, range_end);

                const auto val_end = key_end + (range.val_start - range.key_start);
                const auto val_start = current_pos + (range.val_start - range.key_start);
                val_ranges.push_back(std::array<long long, 2>{ val_start, val_end });
                current_pos = key_end + 1LL;
            }

            // Match the remainder of the range to identity function
            if (current_pos <= end) {
                val_ranges.push_back(std::array<long long, 2>{ current_pos, end });
            }
        }

        return val_ranges;
    }
};

int
main() {  // NOLINT
    const auto lines = utils::read_lines_from_file("input.txt");

    std::vector<std::array<long long, 2>> seed_ranges;
    std::vector<RangeMap> maps;
    RangeMap* current_range_map{ nullptr };

    for (size_t i{ 0 }; i < lines.size(); ++i) {
        const auto& line{ lines.at(i) };
        if (line.contains("seeds:")) {
            const auto name_seeds = utils::split_string(line, ':');
            const auto seeds_str = utils::split_string(name_seeds.at(1), ' ');

            for (size_t i{ 0 }; i < seeds_str.size(); i += 2) {
                const auto start{ std::stoll(seeds_str.at(i)) };
                const auto length{ std::stoll(seeds_str.at(i + 1)) };
                seed_ranges.push_back(std::array<long long, 2>{ start, start + length - 1LL });
            }
        } else if (line.size() < 3) {
            continue;
        } else if (line.contains("map:")) {
            maps.emplace_back();
            current_range_map = &maps.back();
        } else {  // Assuming we have a row of 3 numbers
            const auto numbers_str = utils::split_string(line, ' ');
            if (current_range_map != nullptr) {
                current_range_map->add_range(RangeIndex{ .key_start = std::stoll(numbers_str.at(1)),
                                                         .val_start = std::stoll(numbers_str.at(0)),
                                                         .length = std::stoll(numbers_str.at(2)) });
            } else {
                std::cout << "oh no";
            }
        }
    }

    for (auto& map : maps) {
        map.sort();
    }

    auto final_ranges{ seed_ranges };

    for (auto& map : maps) {
        final_ranges = map.intersect_ranges(final_ranges);
    }

    long long min_location = std::numeric_limits<long long>::max();
    for (const auto& range : final_ranges) {
        if (range.at(0) < min_location) {
            min_location = range.at(0);
        }
    }
    std::cout << min_location << "\n";

    return 0;
}