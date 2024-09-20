#include <Eigen/Dense>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>

#include <Eigen/src/Core/util/Meta.h>

#include "utils/utils.h"

constexpr int STEPS{ 64 };
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
    const Eigen::Index n_rows{ static_cast<Eigen::Index>(lines.size()) };
    const Eigen::Index n_cols{ static_cast<Eigen::Index>(lines[0].size()) };
    const Eigen::Index start_row{ n_rows / 2 };
    const Eigen::Index start_col{ n_cols / 2 };

    Eigen::MatrixXi board = Eigen::MatrixXi::Zero(n_rows, n_cols);
    for (Eigen::Index i{ 0 }; i < n_rows; ++i) {
        for (Eigen::Index j{ 0 }; j < n_cols; ++j) {
            if (lines[static_cast<std::size_t>(i)][static_cast<std::size_t>(j)] == '#') {
                board(i, j) = Rock;
            } else {
                board(i, j) = Garden;
            }
        }
    }

    int suitable{ 0 };
    const auto start{ std::chrono::high_resolution_clock::now() };
    const auto dists{ dijkstra(board, Position{ start_row, start_col }) };
    for (Eigen::Index i{ 0 }; i < n_rows; ++i) {
        for (Eigen::Index j{ 0 }; j < n_cols; ++j) {
            if (dists(i, j) >= 0 && dists(i, j) <= STEPS && dists(i, j) % 2 == PARITY) {
                suitable += 1;
            }
        }
    }

    const auto end{ std::chrono::high_resolution_clock::now() };
    auto ms_int = duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << suitable << "\n";
    std::cout << "Time taken: " << ms_int << " \n";
    return 0;
};
