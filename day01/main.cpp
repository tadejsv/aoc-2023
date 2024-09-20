#include <array>
#include <cctype>
#include <fstream>  // IWYU pragma: keep
#include <iostream>
#include <string>
#include <vector>

int
main() {  // NOLINT
    const std::string fname{ "input.txt" };

    std::ifstream file{ fname };

    if (!file.is_open()) {
        std::cerr << "Can't read the file :("
                  << "\n";
        return 1;
    }

    std::vector<std::array<int, 2>> line_numbers{};

    std::string line;
    while (getline(file, line)) {
        bool gotFirst{ false };
        std::array<int, 2> numbers{};

        for (auto chr : line) {
            if (bool(isdigit(chr))) {
                if (gotFirst) {
                    numbers[1] = chr - '0';
                } else {
                    numbers[0] = chr - '0';
                    numbers[1] = chr - '0';
                    gotFirst = true;
                }
            }
        }

        line_numbers.push_back(numbers);
    }

    int total{ 0 };
    constexpr int first_digit_multiplier{ 10 };
    for (const auto& nums : line_numbers) {
        total += first_digit_multiplier * nums[0] + nums[1];
    }

    std::cout << total;
    return 0;
}