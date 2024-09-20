#include <algorithm>
#include <chrono>
#include <cstddef>
#include <Eigen/Dense>
#include <iostream>
#include <stdexcept>
#include <string_view>
#include <tuple>
#include <vector>

#include <Eigen/src/Core/util/Meta.h>

#include "utils/utils.h"

enum Tile : int { Empty = 0, SlashRight = 1, SlashLeft = 2, Vertical = 3, Horizontal = 4 };

enum Direction : int { Up = 0, Down = 1, Left = 2, Right = 3 };

using Beam = std::tuple<Eigen::Index, Eigen::Index, Direction>;

auto
is_oob(Eigen::Index i_pos, Eigen::Index j_pos, Eigen::Index n_rows, Eigen::Index n_cols) -> bool {
    return (i_pos < 0 || j_pos < 0 || i_pos >= n_rows || j_pos >= n_cols);
};

auto
trace_beam(const Eigen::MatrixXi& board, std::vector<Beam>& beams, Eigen::MatrixXi& visited) -> void {
    const auto current_beam = beams[beams.size() - 1];
    beams.pop_back();

    auto i = std::get<0>(current_beam);  // NOLINT
    auto j = std::get<1>(current_beam);  // NOLINT
    auto direction = std::get<2>(current_beam);

    while (!is_oob(i, j, board.cols(), board.rows())) {
        if ((visited(i, j) & (1 << direction)) != 0) {
            break;
        }

        visited(i, j) |= (1 << direction);

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
                        beams.emplace_back(i - 1, j, Up);
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
                        beams.emplace_back(i, j - 1, Left);
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

auto
full_trace(const Eigen::MatrixXi& board, const Beam& start) -> long {
    std::vector<Beam> beams{ start };
    Eigen::MatrixXi visited = Eigen::MatrixXi::Zero(board.rows(), board.cols());

    while (!beams.empty()) {
        trace_beam(board, beams, visited);
    }

    return visited.count();
}

int
main()  // NOLINT
{
    const auto lines = utils::read_lines_from_file("input.txt");
    const Eigen::Index n_rows{ static_cast<Eigen::Index>(lines.size()) };
    const Eigen::Index n_cols{ static_cast<Eigen::Index>(lines[0].size()) };

    Eigen::MatrixXi board = Eigen::MatrixXi::Zero(n_rows, n_cols);

    for (Eigen::Index i{ 0 }; i < n_rows; ++i) {
        for (Eigen::Index j{ 0 }; j < n_cols; ++j) {
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

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<long> sums{};

    for (Eigen::Index i{ 0 }; i < n_rows; ++i) {
        sums.push_back(full_trace(board, std::make_tuple(i, 0, Right)));
        sums.push_back(full_trace(board, std::make_tuple(i, n_cols - 1, Left)));
    }
    for (Eigen::Index j{ 0 }; j < n_cols; ++j) {
        sums.push_back(full_trace(board, std::make_tuple(0, j, Down)));
        sums.push_back(full_trace(board, std::make_tuple(n_rows - 1, j, Up)));
    }

    const auto max_el = std::ranges::max_element(sums);

    std::cout << *max_el << "\n";
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Clock time: " << duration.count() << " us"
              << "\n";
    return 0;
};
