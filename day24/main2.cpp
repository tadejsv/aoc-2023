// at time t[i] our rock and hailstone intersect, this
// s0 + t[i] * v0 = s[i] + t[i] * v[i]
// s0 - s[i] = t[i] * (v0 - v[i])
// -> as t[i] is a scalar, this means that s0 - s[i] and v0 - v[i] are parallel
// thus (s0 - s[i]) x (v0 - v[i]) = 0
// s0 x v0 - s[i] x v0 - s0 x v[i] + s[i] x v[i] == 0
// this holds for any i, we have 6 unknowns. so with some manipulation we get
// (s[i] - s[1]) x v0 - s0 x (v[i] - v[1]) + (s[i] x v[i] - s[1] x v[1]) = 0
// (s[i] - s[1]) x v0 - s0 x (v[i] - v[1]) = (s[i] x v[i] - s[1] x v[1])
// (s[i] - s[1]) x v0 + (v[i] - v[1]) x s0 = (s[i] x v[i] - s[1] x v[1])

// so we use this equation with i = 2 and i = 3 to get a 6 equations for 6 variables.


#include <Eigen/Dense>
#include <iostream>
#include <string>
#include <vector>

#include "utils/utils.h"

const int NUM_EQUATIONS = 6;

using Vector3ld = Eigen::Matrix<double, 3, 1>;

struct Stone {
    Vector3ld p;
    Vector3ld v;
};

auto
add_cross_coeffs(Eigen::Ref<Eigen::Matrix<double, 3, 3>> block, Vector3ld vec) -> void {
    block(0, 1) = -vec(2);
    block(0, 2) = vec(1);
    block(1, 0) = vec(2);
    block(1, 2) = -vec(0);
    block(2, 0) = -vec(1);
    block(2, 1) = vec(0);
};

int
main() {  // NOLINT
    const auto lines = utils::read_lines_from_file("input.txt");
    std::vector<Stone> stones{};
    for (const auto& line : lines) {
        const auto& parts = utils::split_string(line, '@');
        const auto& pparts{ utils::split_string(parts[0], ',') };
        const auto& vparts{ utils::split_string(parts[1], ',') };
        const Stone stone{
            .p = Vector3ld{ std::stod(utils::trim(pparts[0])),
                            std::stod(utils::trim(pparts[1])),
                            std::stod(utils::trim(pparts[2])) },
            .v = Vector3ld{ std::stod(utils::trim(vparts[0])),
                            std::stod(utils::trim(vparts[1])),
                            std::stod(utils::trim(vparts[2])) },
        };
        stones.push_back(stone);
    }

    Eigen::Matrix<double, NUM_EQUATIONS, 1> rhs;
    rhs.block<3, 1>(0, 0) = stones[0].p.cross(stones[0].v) - stones[1].p.cross(stones[1].v);
    rhs.block<3, 1>(3, 0) = stones[0].p.cross(stones[0].v) - stones[2].p.cross(stones[2].v);

    Eigen::Matrix<double, NUM_EQUATIONS, NUM_EQUATIONS> coeffs;
    add_cross_coeffs(coeffs.block<3, 3>(0, 0), stones[1].p - stones[0].p);
    add_cross_coeffs(coeffs.block<3, 3>(3, 0), stones[2].p - stones[0].p);
    add_cross_coeffs(coeffs.block<3, 3>(0, 3), stones[1].v - stones[0].v);
    add_cross_coeffs(coeffs.block<3, 3>(3, 3), stones[2].v - stones[0].v);

    const auto solution{ coeffs.inverse() * rhs };
    std::cout << solution << "\n\n";
    std::cout << static_cast<long long>(solution.segment(3, 3).sum()) << "\n";

    return 0;
};
