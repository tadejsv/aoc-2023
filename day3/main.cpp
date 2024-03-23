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

using Eigen::MatrixXi;

struct Digit {
    int digit;
    std::size_t length;
    std::array<Eigen::Index, 2> index;
};

auto get_symbols(const std::vector<std::string> &lines) -> MatrixXi {
    const Eigen::Index height{static_cast<Eigen::Index>(lines.size())};
    const Eigen::Index width{static_cast<Eigen::Index>(lines.at(0).length())};

    MatrixXi symbols(height, width);
    symbols.setZero();

    for (Eigen::Index i = 0; i < height; ++i) {
        std::string_view line{lines.at(static_cast<std::size_t>(i))};
        for (Eigen::Index j = 0; j < width; ++j) {
            const auto character = line.at(static_cast<std::size_t>(j));
            if (!static_cast<bool>(isdigit(character)) && character != '.') {
                symbols(i, j) = 1;
            }
        }
    }

    return symbols;
}

auto get_digits(const std::vector<std::string> &lines) -> std::vector<Digit> {
    const Eigen::Index height{static_cast<Eigen::Index>(lines.size())};
    const Eigen::Index width{static_cast<Eigen::Index>(lines.at(0).length())};

    std::vector<Digit> digits;

    for (Eigen::Index i = 0; i < height; ++i) {
        std::string_view line{lines.at(static_cast<std::size_t>(i))};
        std::string current_digits;
        for (Eigen::Index j = 0; j < width; ++j) {
            const auto character = line.at(static_cast<std::size_t>(j));
            if (static_cast<bool>(isdigit(character))) {
                current_digits += character;
            } else if (current_digits.length() > 0) {
                // Digit has ended, mark it down
                Digit new_digit{std::stoi(current_digits),
                    current_digits.length(),
                    std::array<Eigen::Index, 2>{i, j - 1}};
                digits.push_back(new_digit);
                current_digits = "";
            }
        }

        if (current_digits.length() > 0) {
            // Digit has ended, mark it down
            Digit new_digit{std::stoi(current_digits),
                current_digits.length(),
                std::array<Eigen::Index, 2>{i, height - 1}};
            digits.push_back(new_digit);
            current_digits = "";
        }
    }

    return digits;
}

int main() { // NOLINT
    const auto lines = utils::read_lines_from_file("input.txt");

    const auto symbols = get_symbols(lines);
    const auto digits = get_digits(lines);

    int parts_sum{0};
    for (auto const digit : digits) {
        Eigen::Index y_start = std::max(digit.index.at(0) - 1L, 0L);
        Eigen::Index x_start =
            std::max(digit.index.at(1) - static_cast<Eigen::Index>(digit.length), 0L);
        Eigen::Index y_end = std::min(digit.index.at(0) + 1L, symbols.rows() - 1L);
        Eigen::Index x_end = std::min(digit.index.at(1) + 1L, symbols.cols() - 1L);

        auto sub_block =
            symbols.block(y_start, x_start, y_end - y_start + 1L, x_end - x_start + 1L);

        if (sub_block.maxCoeff() > 0) {
            std::cout << digit.digit << "\n";
            parts_sum += digit.digit;
        }
    }

    std::cout << parts_sum << "\n";
}
