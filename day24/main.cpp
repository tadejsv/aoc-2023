#include <iostream>
#include <string>
#include <vector>

#include "utils/utils.h"

template <typename T>
auto
sign(T val) -> T {
    return (T(0) < val) - (val < T(0));
}

struct Stone {
    double px, py, pz;
    double vx, vy, vz;
};

// const double LOWER_LIMIT = 7;
// const double UPPER_LIMIT = 27;
const double LOWER_LIMIT = 200000000000000;
const double UPPER_LIMIT = 400000000000000;

int
main() {  // NOLINT
    const auto lines = utils::read_lines_from_file("input.txt");
    std::vector<Stone> stones{};
    for (const auto& line : lines) {
        const auto& parts = utils::split_string(line, '@');
        const auto& pparts{ utils::split_string(parts[0], ',') };
        const auto& vparts{ utils::split_string(parts[1], ',') };
        const Stone stone{
            .px = std::stod(utils::trim(pparts[0])),
            .py = std::stod(utils::trim(pparts[1])),
            .pz = std::stod(utils::trim(pparts[2])),
            .vx = std::stod(utils::trim(vparts[0])),
            .vy = std::stod(utils::trim(vparts[1])),
            .vz = std::stod(utils::trim(vparts[2])),
        };
        stones.push_back(stone);
    }

    int intersections{ 0 };
    for (std::size_t i{ 0 }; i < stones.size(); ++i) {
        const auto& istone{ stones[i] };
        const double k_i{ istone.vy / istone.vx };
        const double y0_i{ istone.py - k_i * istone.px };
        for (std::size_t j{ i + 1 }; j < stones.size(); ++j) {
            const auto& jstone{ stones[j] };
            const double k_j{ jstone.vy / jstone.vx };
            const double y0_j{ jstone.py - k_j * jstone.px };

            if (k_j == k_i) {
                continue;
            }

            double int_x{ (y0_i - y0_j) / (k_j - k_i) };
            double int_y{ y0_i + k_i * int_x };
            bool x_ok{ int_x > LOWER_LIMIT && int_x < UPPER_LIMIT };
            bool y_ok{ int_y > LOWER_LIMIT && int_y < UPPER_LIMIT };
            if (sign(istone.vx) * (int_x - istone.px) >= 0
                && sign(jstone.vx) * (int_x - jstone.px) >= 0 && x_ok && y_ok) {
                intersections++;
            }
        }
    }

    std::cout << intersections << "\n";

    return 0;
};
