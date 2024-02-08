#include "utils/utils.h"
#include <cstdlib>
#include <iostream>
#include <string_view>
#include <vector>

struct Star {
    long long x{0};
    long long y{0};

    [[nodiscard]] auto dist(const Star &other) const -> long long {
        return std::abs(x - other.x) + std::abs(y - other.y);
    };
};

int main() { // NOLINT
    constexpr long long million{999999};
    const auto lines = utils::read_lines_from_file("input.txt");

    const auto height{lines.size()};
    const auto width{lines.at(0).size()};

    std::vector<int> empty_cols(width, 1);
    std::vector<int> empty_rows(height, 1);

    std::vector<Star> stars{};

    for (std::size_t i{0}; i < height; i++) {
        std::string_view line{lines.at(i)};
        for (std::size_t j{0}; j < width; j++) {
            if (line.at(j) == '#') {
                stars.emplace_back(j, i);
                empty_cols[j] = 0;
                empty_rows[i] = 0;
            }
        }
    }

    std::vector<long long> expand_cols(width, 0);
    std::vector<long long> expand_rows(height, 0);
    for (std::size_t i{1}; i < height; i++) {
        expand_rows[i] = expand_rows[i - 1] + empty_rows[i] * million;
    }
    for (std::size_t i{1}; i < width; i++) {
        expand_cols[i] = expand_cols[i - 1] + empty_cols[i] * million;
    }

    for (auto &star : stars) {
        star.y += expand_rows[static_cast<std::size_t>(star.y)];
        star.x += expand_cols[static_cast<std::size_t>(star.x)];
    }

    long long sum_dists{0};
    for (std::size_t s1{0}; s1 < stars.size() - 1; s1++) {
        for (std::size_t s2{s1}; s2 < stars.size(); s2++) {
            sum_dists += stars[s1].dist(stars[s2]);
        }
    }

    std::cout << sum_dists << "\n";
}