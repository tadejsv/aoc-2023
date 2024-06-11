#include "utils/utils.h"
#include <Eigen/Dense>
#include <Eigen/src/Core/util/Meta.h>
#include <cstddef>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string_view>
#include <tuple>
#include <vector>

enum Tile : int {
    Empty = 0,
    SlashRight = 1,
    SlashLeft = 2,
    Vertical = 3,
    Horizontal = 4
};
enum Direction : int { Up = 0, Down = 1, Left = 2, Right = 3 };

struct Beam {
    std::tuple<Eigen::Index, Eigen::Index> start;
    Direction dir;
};

using VisistedSet = std::set<std::tuple<Eigen::Index, Eigen::Index, Direction>>;

auto is_oob(
    Eigen::Index i_pos, Eigen::Index j_pos, Eigen::Index n_rows, Eigen::Index n_cols)
    -> bool {
    return (i_pos < 0 || j_pos < 0 || i_pos >= n_rows || j_pos >= n_cols);
};

auto trace_beam(const Eigen::MatrixXi &board,
    Eigen::MatrixXi &energized,
    std::vector<Beam> &beams,
    VisistedSet &visited) -> void {

    const auto current_beam = beams[beams.size() - 1];
    beams.pop_back();

    auto [i, j] = current_beam.start;
    auto direction = current_beam.dir;

    while (!is_oob(i, j, board.cols(), board.rows())) {
        if (visited.contains(std::make_tuple(i, j, direction))) {
            break;
        }

        energized(i, j) = 1;
        visited.emplace(i, j, direction);

        // Change or keep direction based on tile
        switch (static_cast<Tile>(board(i, j))) {
        case Empty:
            break;
        case SlashRight:
            switch (direction) {
            case Right:
                direction = Up;
                break;
            case Left:
                direction = Down;
                break;
            case Up:
                direction = Right;
                break;
            case Down:
                direction = Left;
                break;
            }
            break;
        case SlashLeft:
            switch (direction) {
            case Right:
                direction = Down;
                break;
            case Left:
                direction = Up;
                break;
            case Up:
                direction = Left;
                break;
            case Down:
                direction = Right;
                break;
            }
            break;
        case Vertical:
            switch (direction) {
            case Right:
            case Left:
                direction = Down;
                beams.emplace_back(
                    std::make_tuple(Eigen::Index{i - 1}, Eigen::Index{j}), Up);
                break;
            case Up:
            case Down:
                break;
            }
            break;
        case Horizontal:
            switch (direction) {
            case Right:
            case Left:
                break;
            case Up:
            case Down:
                direction = Right;
                beams.emplace_back(
                    std::make_tuple(Eigen::Index{i}, Eigen::Index{j - 1}), Left);
                break;
            }
            break;
        }

        switch (direction) {
        case Up:
            --i;
            break;
        case Down:
            ++i;
            break;
        case Left:
            --j;
            break;
        case Right:
            ++j;
            break;
        }
    }
};

int main() // NOLINT
{
    const auto lines = utils::read_lines_from_file("input.txt");
    const Eigen::Index n_rows{static_cast<Eigen::Index>(lines.size())};
    const Eigen::Index n_cols{static_cast<Eigen::Index>(lines[0].size())};

    Eigen::MatrixXi board = Eigen::MatrixXi::Zero(n_rows, n_cols);
    Eigen::MatrixXi energized = Eigen::MatrixXi::Zero(n_rows, n_cols);
    VisistedSet visited{};

    for (Eigen::Index i{0}; i < n_rows; ++i) {
        for (Eigen::Index j{0}; j < n_cols; ++j) {
            switch (lines[static_cast<std::size_t>(i)][static_cast<std::size_t>(j)]) {
            case '.':
                board(i, j) = Empty;
                break;
            case '/':
                board(i, j) = SlashRight;
                break;
            case '\\':
                board(i, j) = SlashLeft;
                break;
            case '|':
                board(i, j) = Vertical;
                break;
            case '-':
                board(i, j) = Horizontal;
                break;
            default:
                throw std::invalid_argument("Unknown board element");
            }
        }
    }

    std::vector<Beam> beams{{std::make_tuple(Eigen::Index{0}, Eigen::Index{0}), Right}};

    while (!beams.empty()) {
        trace_beam(board, energized, beams, visited);
    }

    std::cout << energized.sum() << "\n";
    return 0;
};
