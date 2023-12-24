#include "day6.h"
#include <cmath>

auto num_wins(const long long total_time, const long long record) -> long long {
    auto const square_coeff = total_time * total_time - 4 * record;
    if (square_coeff < 0L) {
        return 0L;
    }

    // Subtracting epsilon to make  sure we always get more than record
    auto const root_coeff{std::sqrt(square_coeff) - 0.00001};
    auto const middle{static_cast<double>(total_time) / 2.0};

    auto const start{middle - root_coeff / 2.0};
    auto const end{middle + root_coeff / 2.0};

    auto const starti{static_cast<long long>(std::ceil(start))};
    auto const endi{static_cast<long long>(std::floor(end))};

    return endi - starti + 1L;
}
