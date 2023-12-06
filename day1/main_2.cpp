#include <array>
#include <cctype>
#include <fstream> // IWYU pragma: keep
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// This function takes a file where lines contain a mix of numeric and alphabet
// characters, among which could also be words for the numbers up to 10 (zero,
// one, ..., nine). An example of such line is "asbone2asnine2". It parses the
// line for all numbers (both digits and words), and takes the first and last
// number of each line, concatenates them together (e.g. (2, 5) -> 25), and sums
// up the numbers for all the lines. It then prints out the final sum.
auto main() -> int {
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
    int digit{};
    int last_digit_pos{0};
    while (getline(file, line)) {
        bool gotFirst{false};
        std::array<int, 2> numbers{};

        for (size_t i = 1; i <= line.size(); ++i) {
            std::cout << line.at(i);
            continue;
            // if (bool(isdigit(line.at(i)))) {
            //     digit = chr - '0';
            // } else {
            // }

            // if (digit >= 0) {
            //     if (gotFirst) {
            //         numbers[1] = digit;
            //     } else {
            //         numbers[0] = digit;
            //         numbers[1] = digit;
            //         gotFirst = true;
            //     }
            // }
        }

        // line_numbers.push_back(numbers);
    }

    // int total{0};
    // for (const auto &nums : line_numbers) {
    //     total += 10 * nums[0] + nums[1];
    // }

    // std::cout << total;
    return 0;
}