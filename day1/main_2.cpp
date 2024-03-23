#include <array>
#include <cctype>
#include <cstddef>
#include <fstream> // IWYU pragma: keep
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

auto find_digit(const std::string &str,
    std::size_t start,
    std::size_t end,
    std::size_t substr_length,
    const std::unordered_map<std::string, int> &digit_map) -> int {
    if (end - start + 1 >= substr_length) {
        auto substr = str.substr(end - substr_length + 1, substr_length);
        std::cout << substr << "\n";
        auto num_ret = digit_map.find(substr);
        if (num_ret != digit_map.end()) {
            return num_ret->second;
        }
    }
    return -1;
};

// This function takes a file where lines contain a mix of numeric and alphabet
// characters, among which could also be words for the numbers up to 10 (zero,
// one, ..., nine). An example of such line is "asbone2asnine2". It parses the
// line for all numbers (both digits and words), and takes the first and last
// number of each line, concatenates them together (e.g. (2, 5) -> 25), and sums
// up the numbers for all the lines. It then prints out the final sum.
auto main() -> int { // NOLINT
    const std::string fname{"../../day1/input.txt"};

    std::ifstream file{fname};

    const std::unordered_map<std::string, int> three_letter_nums{
        {"one", 1},
        {"two", 2},
        {"six", 6},
    };
    const std::unordered_map<std::string, int> four_letter_nums{
        {"zero", 0},
        {"four", 4},
        {"five", 5},
        {"nine", 9},
    };
    const std::unordered_map<std::string, int> five_letter_nums{
        {"three", 3},
        {"seven", 7},
        {"eight", 8},
    };

    if (!file.is_open()) {
        std::cerr << "Can't read the input file :("
                  << "\n";
        return 1;
    }

    std::vector<std::array<int, 2>> line_numbers{};

    std::string line;
    while (getline(file, line)) {
        std::array<int, 2> numbers{};
        bool gotFirst{false};
        std::size_t last_digit_pos{0};

        for (size_t i = 0; i < line.length(); ++i) {
            int digit{-1};
            auto chr = line.at(i);

            if (bool(isdigit(chr))) {
                digit = chr - '0';
                last_digit_pos = i;
            } else {
                if (auto three_digit =
                        find_digit(line, last_digit_pos, i, 3, three_letter_nums);
                    three_digit >= 0) {
                    digit = three_digit;
                }
                if (auto four_digit =
                        find_digit(line, last_digit_pos, i, 4, four_letter_nums);
                    four_digit >= 0) {
                    digit = four_digit;
                }

                constexpr std::size_t len5{5};
                if (auto five_digit =
                        find_digit(line, last_digit_pos, i, len5, five_letter_nums);
                    five_digit >= 0) {
                    digit = five_digit;
                }
            }

            if (digit >= 0) {
                if (gotFirst) {
                    numbers[1] = digit;
                } else {
                    numbers[0] = digit;
                    numbers[1] = digit;
                    gotFirst = true;
                }
            }
        }
        std::cout << numbers[0] << " " << numbers[1] << "\n";
        line_numbers.push_back(numbers);
    }

    int total{0};
    constexpr int first_digit_multiplier{10};
    for (const auto &nums : line_numbers) {
        total += first_digit_multiplier * nums[0] + nums[1];
    }

    std::cout << total;
    return 0;
}