#include "utils/utils.h"
#include <Eigen/Dense>
#include <Eigen/src/Core/util/Meta.h>
#include <cstddef>
#include <iostream>
#include <queue>
#include <string_view>
#include <unordered_set>
#include <vector>

constexpr int min_steps{4};
constexpr int max_steps{10};

enum Direction : int { Up = 0, Down = 1, Left = 2, Right = 3 };
struct Position {
    Eigen::Index row;
    Eigen::Index col;
    int steps;
    Direction dir;

    auto operator==(const Position &other) const -> bool {
        return row == other.row && col == other.col && steps == other.steps &&
               dir == other.dir;
    }
};

struct PositionHash {
    auto operator()(const Position &p) const -> std::size_t {         // NOLINT
        return (p.row << 20) | (p.col << 8) | (p.steps << 2) | p.dir; // NOLINT
    }
};

struct PostHeat {
    Position pos;
    int heat;
};

class CompareHeat {
  public:
    auto operator()(const PostHeat &pos1, const PostHeat &pos2) const -> bool {
        return pos1.heat > pos2.heat;
    }
};

auto is_valid(const Position &pos,
    const Eigen::Index nrows,
    const Eigen::Index ncols,
    int prev_steps) -> bool {
    if (pos.steps > max_steps || (pos.steps == 1 && prev_steps < min_steps)) {
        return false;
    }
    if (pos.row + 1 > nrows || pos.col + 1 > ncols || pos.row < 0 || pos.col < 0) {
        return false;
    }

    return true;
};

int main() // NOLINT
{
    const auto lines = utils::read_lines_from_file("input.txt");
    const Eigen::Index n_rows{static_cast<Eigen::Index>(lines.size())};
    const Eigen::Index n_cols{static_cast<Eigen::Index>(lines[0].size())};

    Eigen::MatrixXi board = Eigen::MatrixXi::Zero(n_rows, n_cols);

    for (Eigen::Index i{0}; i < n_rows; ++i) {
        for (Eigen::Index j{0}; j < n_cols; ++j) {
            board(i, j) =
                lines[static_cast<std::size_t>(i)][static_cast<std::size_t>(j)] - '0';
        }
    }

    std::unordered_set<Position, PositionHash> considered{};
    std::priority_queue<PostHeat, std::vector<PostHeat>, CompareHeat> min_queue{};
    min_queue.emplace(Position{1, 0, 1, Down}, board(1, 0));
    min_queue.emplace(Position{0, 1, 1, Right}, board(0, 1));
    considered.emplace(1, 0, 1, Down);
    considered.emplace(0, 1, 1, Right);
    int min_heat{-1};

    const auto start{std::chrono::high_resolution_clock::now()};
    while (!min_queue.empty()) {
        const auto current{min_queue.top()};
        min_queue.pop();
        const auto curr_row{current.pos.row};
        const auto curr_col{current.pos.col};
        const auto curr_steps{current.pos.steps};

        if (curr_col + 1 == n_cols && curr_row + 1 == n_rows &&
            curr_steps >= min_steps) {
            min_heat = current.heat;
            break;
        }

        std::array<Position, 3> next{};
        switch (current.pos.dir) {
        case Up:
            next[0] = {curr_row - 1, curr_col, curr_steps + 1, Up};
            next[1] = {curr_row, curr_col - 1, 1, Left};
            next[2] = {curr_row, curr_col + 1, 1, Right};
            break;
        case Down:
            next[0] = {curr_row + 1, curr_col, curr_steps + 1, Down};
            next[1] = {curr_row, curr_col - 1, 1, Left};
            next[2] = {curr_row, curr_col + 1, 1, Right};
            break;
        case Left:
            next[0] = {curr_row, curr_col - 1, curr_steps + 1, Left};
            next[1] = {curr_row + 1, curr_col, 1, Down};
            next[2] = {curr_row - 1, curr_col, 1, Up};
            break;
        case Right:
            next[0] = {curr_row, curr_col + 1, curr_steps + 1, Right};
            next[1] = {curr_row + 1, curr_col, 1, Down};
            next[2] = {curr_row - 1, curr_col, 1, Up};
            break;
        }

        for (const auto &next_pos : next) {
            if (is_valid(next_pos, n_rows, n_cols, curr_steps)) {
                if (!considered.contains(next_pos)) {
                    considered.insert(next_pos);
                    min_queue.emplace(
                        next_pos, current.heat + board(next_pos.row, next_pos.col));
                }
            }
        }
    }
    const auto end{std::chrono::high_resolution_clock::now()};
    auto ms_int = duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << min_heat << "\n";
    std::cout << "Time taken: " << ms_int << " \n";

    return 0;
};
