#include <cmath>
#include <Eigen/Dense>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>

#include <Eigen/src/Core/Matrix.h>

#include "utils/utils.h"

// ⚠️ NOTE: this will not work on the test case, as it does not match the assumptions
// of the actual case, which we are using to simplify the calculations
//
// This solution uses the fact that the vertical and horizontal line at the starting
// point, as well as all edge lines on the original square, are free of rocks.
// This, together with taxicab-distance property of this puzzle, means we can reduce
// this puzzle to checking distances from a small set of starting points in the
// original square, and don't need to work with the expanded version.
//
// We can split the squares in the expanded version in 3 types:
// * original square (at the middle of everything)
// * straight squares (squares that are in a straight vertical or horizontal line
//   from original quare)
// * corner squares (squares that are in one of the "corners" from the original squares)
//
// The illustration below shows the entry points for straight squares (T cells), corner
// squares (C cells) and the original square (S).
//
// C * T * C
// * * * * *
// T * S * T
// * * * * *
// C * T * C
//
// For the original square, we simply take all the points with the same parity (odd) as
// the number of steps (N_S), with distance measured from the starting point.
//
// The straight squares will be approached in a straight line from the starting point. The
// distance to a point in such a square from the starting point will therefore be
//     R + k*N + D_M
// where R is distance from starting point to the edge, N is the length of the square side.
// k is the number of squares we have to travel from the starting point, and D_M is the
// distance from the starting point of the square to the point.
//
// We have that R = ceil(N/2) = 66, which is even.
//
// Taken together, this means that we get two sets of reachable points:
// * points with odd parity for D_M: we get COUNT of even integers in interval
//       [0, U], where U = floor((N_S - R - D_M)/N)
//   this is because only with even k will R + k*N + D_M be odd. This is
//   floor(U/2)+1
// * points with even parity for D_M: we get COUNT of odd integers in interval
//       [0, U], where U = floor((N_S - R - D_M)/N)
//   this is because only with odd k will R + k*N + D_M be odd. This is
//   floor((U+1)/2)
//
// For corner cells, we have a similar situation - a distance from starting point to one
// of the points in a corner square is
//    k*N + D_M
// We have two cases here:
// * points with odd parity for D_M: we get SUM of even integers in interval
//       [1, U], where U = floor((N_S - D_M)/N)
//   this is because only with even k will k*N + D_M be odd. Let s = floor(U/2),
//   then we have s * (s+1)
// * points with even parity for D_M: we get the SUM of odd integers in interval
//       [1, U], where U = floor((N_S - D_M)/N)
//   this is because only with odd k will k*N + D_M be odd. Let s = floor((U+1)/2),
//   then we have s * s
//
// Note that here we have the sum - this is because unlike in the straight squares
// case, the number of squares grows with the distance (quadratically), as we are
// moving not in a straight line, but in a plane.


constexpr int STEPS{ 26501365 };
constexpr int PARITY{ STEPS % 2 };

enum Tile : int { Garden = 0, Rock = 1 };

using Position = std::pair<Eigen::Index, Eigen::Index>;

struct PositionHash {
    auto operator()(const Position& p) const -> std::size_t {           // NOLINT
        return static_cast<std::size_t>((p.first << 12) | (p.second));  // NOLINT
    }
};

struct PositionDist {
    Position pos;
    int dist;
};

class ComparePositionDist {
public:

    auto operator()(const PositionDist& pos1, const PositionDist& pos2) const -> bool {
        return pos1.dist > pos2.dist;
    }
};

const std::array<std::pair<Eigen::Index, Eigen::Index>, 4> directions{
    std::make_pair(1, 0),   // down
    std::make_pair(-1, 0),  // up
    std::make_pair(0, 1),   // right
    std::make_pair(0, -1)   // left
};

auto
dijkstra(const Eigen::MatrixXi& board, Position start) -> Eigen::MatrixXi {
    const Eigen::Index n_rows{ board.rows() };
    const Eigen::Index n_cols{ board.cols() };

    auto dists{ Eigen::MatrixXi(n_rows, n_cols) };
    dists.setConstant(-1);

    std::unordered_set<Position, PositionHash> considered{};
    std::priority_queue<PositionDist, std::vector<PositionDist>, ComparePositionDist> min_queue{};
    min_queue.emplace(Position{ start.first, start.second }, 0);

    while (!min_queue.empty()) {
        const auto current{ min_queue.top() };
        min_queue.pop();
        if (considered.contains(current.pos)) {
            continue;
        }
        considered.insert(current.pos);

        const auto cur_row{ current.pos.first };
        const auto cur_col{ current.pos.second };

        dists(cur_row, cur_col) = current.dist;

        for (const auto& [d_row, d_col] : directions) {
            const Eigen::Index new_row{ cur_row + d_row };
            const Eigen::Index new_col{ cur_col + d_col };

            if (new_col < n_cols && new_col >= 0 && new_row < n_rows && new_row >= 0
                && board(new_row, new_col) != Rock
                && !considered.contains(Position(new_row, new_col))) {
                min_queue.emplace(Position{ new_row, new_col }, current.dist + 1);
            }
        }
    }

    return dists;
};

int
main() {  // NOLINT
    const auto lines = utils::read_lines_from_file("input.txt");

    // Sides are equal - we are operating with squares
    const Eigen::Index N{ static_cast<Eigen::Index>(lines.size()  // NOLINT(readability-identifier-length)
    ) };
    const Eigen::Index R{ static_cast<Eigen::Index>(  // NOLINT(readability-identifier-length)
        std::ceil(static_cast<double>(N) / 2)
    ) };
    Eigen::MatrixXi board = Eigen::MatrixXi::Zero(N, N);
    for (Eigen::Index i{ 0 }; i < N; ++i) {
        for (Eigen::Index j{ 0 }; j < N; ++j) {
            if (lines[static_cast<std::size_t>(i)][static_cast<std::size_t>(j)] == '#') {
                board(i, j) = Rock;
            } else {
                board(i, j) = Garden;
            }
        }
    }

    const auto start{ std::chrono::high_resolution_clock::now() };
    long long suitable{ 0 };

    // Original square
    const auto dists_orig = dijkstra(board, Position{ R - 1, R - 1 });
    for (Eigen::Index i{ 0 }; i < N; ++i) {
        for (Eigen::Index j{ 0 }; j < N; ++j) {
            if (dists_orig(i, j) % 2 == PARITY && dists_orig(i, j) >= 0) {
                suitable += 1;
            }
        }
    }
    std::cout << suitable << "\n";

    // Straight squares
    std::array<Position, 4> straight_starts{
        Position{ 0, R - 1 },
        Position{ R - 1, 0 },
        Position{ N - 1, R - 1 },
        Position{ R - 1, N - 1 },
    };

    for (const auto& start : straight_starts) {
        const auto dists_straight = dijkstra(board, start);
        for (Eigen::Index i{ 0 }; i < N; ++i) {
            for (Eigen::Index j{ 0 }; j < N; ++j) {
                const auto dist{ dists_straight(i, j) };
                if (dist < 0) {
                    continue;
                }
                const auto parity{ dist % 2 };
                const auto U{ static_cast<long>(  // NOLINT(readability-identifier-length)
                    std::floor(static_cast<double>(STEPS - R - dist) / static_cast<double>(N))
                ) };

                if (parity == 0) {
                    suitable += static_cast<long>(std::floor(static_cast<double>(U + 1) / 2));
                } else {
                    suitable += 1 + static_cast<long>(std::floor(static_cast<double>(U) / 2));
                }
            }
        }
    }
    std::cout << suitable << "\n";

    // Corner squares
    std::array<Position, 4> corner_starts{
        Position{ 0, 0 },
        Position{ 0, N - 1 },
        Position{ N - 1, 0 },
        Position{ N - 1, N - 1 },
    };
    for (const auto& start : corner_starts) {
        const auto dists_corner = dijkstra(board, start);
        for (Eigen::Index i{ 0 }; i < N; ++i) {
            for (Eigen::Index j{ 0 }; j < N; ++j) {
                const auto dist{ dists_corner(i, j) };
                if (dist < 0) {
                    continue;
                }

                const auto parity{ dist % 2 };
                const auto U{ static_cast<long>(  // NOLINT(readability-identifier-length)
                    std::floor(static_cast<double>(STEPS - dist) / static_cast<double>(N))
                ) };

                if (parity == 0) {
                    const auto s{ static_cast<long>(  // NOLINT(readability-identifier-length)
                        std::floor(static_cast<double>(U) / 2)
                    ) };
                    suitable += s * (s + 1);
                } else {
                    const auto s{ static_cast<long>(  // NOLINT(readability-identifier-length)
                        std::floor(static_cast<double>(U + 1) / 2)
                    ) };
                    suitable += s * s;
                }
            }
        }
    }


    const auto end{ std::chrono::high_resolution_clock::now() };
    auto ms_int = duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << suitable << "\n";
    std::cout << "Time taken: " << ms_int << " \n";
    return 0;
};
