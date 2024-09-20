#include <cmath>

#include "day6.h"

auto
num_wins(const long long total_time, const long long record) -> long long {
    const auto square_coeff = total_time * total_time - 4 * record;
    if (square_coeff < 0L) {
        return 0L;
    }

    // Subtracting epsilon to make  sure we always get more than record
    const auto root_coeff{ std::sqrt(square_coeff) - 0.00001 };
    const auto middle{ static_cast<double>(total_time) / 2.0 };

    const auto start{ middle - root_coeff / 2.0 };
    const auto end{ middle + root_coeff / 2.0 };

    const auto starti{ static_cast<long long>(std::ceil(start)) };
    const auto endi{ static_cast<long long>(std::floor(end)) };

    return endi - starti + 1L;
}
