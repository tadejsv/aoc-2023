#include "utils/utils.h"
#include <Eigen/Dense>
#include <algorithm>
#include <array>
#include <cctype> // For isdigit
#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

// Use vector of some length to store digits by lines;

using Eigen::MatrixXi;

auto interval_intersect(Eigen::Index left_1, Eigen::Index right_1, Eigen::Index left_2,
                        Eigen::Index right_2) -> bool {
    return left_1 <= right_2 && left_2 <= right_1;
};

struct Digit {
    int digit;
    std::size_t length;
    std::array<Eigen::Index, 2> index;

    auto start_x() const -> Eigen::Index {
        return index[1] - static_cast<Eigen::Index>(length) + 1L;
    }
};

auto get_gears(const std::vector<std::string> &lines)
    -> std::vector<std::array<Eigen::Index, 2>> {
    const Eigen::Index height{static_cast<Eigen::Index>(lines.size())};
    const Eigen::Index width{static_cast<Eigen::Index>(lines.at(0).length())};

    std::vector<std::array<Eigen::Index, 2>> gears;

    for (Eigen::Index i = 0; i < height; ++i) {
        std::string_view line{lines.at(static_cast<std::size_t>(i))};
        for (Eigen::Index j = 0; j < width; ++j) {
            const auto character = line.at(static_cast<std::size_t>(j));
            if (character == '*') {
                gears.push_back(std::array<Eigen::Index, 2>{i, j});
            }
        }
    }

    return gears;
}

auto get_digits(const std::vector<std::string> &lines)
    -> std::vector<std::vector<Digit>> {
    const Eigen::Index height{static_cast<Eigen::Index>(lines.size())};
    const Eigen::Index width{static_cast<Eigen::Index>(lines.at(0).length())};

    std::vector<std::vector<Digit>> digits;

    for (Eigen::Index i = 0; i < height; ++i) {
        std::vector<Digit> line_digits;

        std::string_view line{lines.at(static_cast<std::size_t>(i))};
        std::string current_digits;
        for (Eigen::Index j = 0; j < width; ++j) {
            const auto character = line.at(static_cast<std::size_t>(j));
            if (static_cast<bool>(isdigit(character))) {
                current_digits += character;
            } else if (current_digits.length() > 0) {
                // Digit has ended, mark it down
                Digit new_digit{std::stoi(current_digits), current_digits.length(),
                                std::array<Eigen::Index, 2>{i, j - 1}};
                line_digits.push_back(new_digit);
                current_digits = "";
            }
        }

        if (current_digits.length() > 0) {
            // Digit has ended, mark it down
            Digit new_digit{std::stoi(current_digits), current_digits.length(),
                            std::array<Eigen::Index, 2>{i, height - 1}};
            line_digits.push_back(new_digit);
            current_digits = "";
        }

        digits.push_back(line_digits);
    }

    return digits;
}

int main() { // NOLINT
    const auto lines = utils::read_lines_from_file("input.txt");
    Eigen::Index num_lines{static_cast<Eigen::Index>(lines.size())};

    const auto gears = get_gears(lines);
    const auto digits = get_digits(lines);

    int sum_gears{0};
    for (const auto &gear : gears) {
        int digit_product{1};
        int num_digits{0};

        for (auto i = std::max(gear[0] - 1L, 0L);
             i <= std::min(gear[0] + 1L, num_lines); i++) {
            auto const &line_digits = digits[i];
            for (auto const &digit : line_digits) {
                if (interval_intersect(digit.start_x(), digit.index[1], gear[1] - 1L,
                                       gear[1] + 1L)) {
                    digit_product *= digit.digit;
                    num_digits += 1;
                }
            }
        }

        if (num_digits == 2) {
            sum_gears += digit_product;
        }
    }

    std::cout << sum_gears;
    return 0;
}
