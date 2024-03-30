#include "utils/utils.h"
#include <Eigen/Core>
#include <Eigen/src/Core/util/Meta.h>
#include <cstddef>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

enum RockType : int { None = 0, Round = 1, Cube = 2 };
enum Direction : int { North = 0, West = 1, South = 2, East = 3 };

class Board {
  private:
    Eigen::MatrixXi board;

  public:
    Board(const std::vector<std::string> &lines) {
        const Eigen::Index n_rows{static_cast<Eigen::Index>(lines.size())};
        const Eigen::Index n_cols{static_cast<Eigen::Index>(lines[0].size())};

        board = Eigen::MatrixXi::Zero(n_rows, n_cols);

        for (Eigen::Index i{0}; i < n_rows; ++i) {
            for (Eigen::Index j{0}; j < n_rows; ++j) {
                if (lines[static_cast<std::size_t>(i)][static_cast<std::size_t>(j)] ==
                    'O') {
                    board(i, j) = Round;
                } else if (lines[static_cast<std::size_t>(i)]
                                [static_cast<std::size_t>(j)] == '#') {
                    board(i, j) = Cube;
                }
            }
        }
    }

    // Tilt the board so that rocks slide as far to a side as possible
    auto tilt(Direction dir) -> void {
        Eigen::Index max_i{board.cols()};
        Eigen::Index max_j{board.rows()};
        if (dir == West || dir == East) {
            max_i = board.rows();
            max_j = board.cols();
        }

        for (Eigen::Index i{0}; i < max_i; ++i) {
            Eigen::Index current_offset{0};
            for (Eigen::Index j{0}; j < max_j; ++j) {
                // Default case for north
                Eigen::Index board_i{j};
                Eigen::Index board_j{i};
                Eigen::Index offset_i{current_offset};
                Eigen::Index offset_j{board_j};

                if (dir == South) {
                    board_i = max_j - j - 1;
                    board_j = max_i - i - 1;
                    offset_i = max_j - current_offset - 1;
                    offset_j = board_j;
                } else if (dir == West) {
                    board_i = i;
                    board_j = j;
                    offset_i = board_i;
                    offset_j = current_offset;
                } else if (dir == East) {
                    board_i = max_i - i - 1;
                    board_j = max_j - j - 1;
                    offset_i = board_i;
                    offset_j = max_j - current_offset - 1;
                }

                if (board(board_i, board_j) == Round) {
                    board(board_i, board_j) = None;
                    board(offset_i, offset_j) = Round;
                    current_offset += 1;
                } else if (board(board_i, board_j) == Cube) {
                    current_offset = j + 1;
                }
            }
        }
    };

    auto hash() -> std::size_t {
        std::size_t seed = 0;

        for (int i = 0; i < board.rows(); ++i) {
            for (int j = 0; j < board.cols(); ++j) {
                // Combine the current element with the running hash.
                seed ^= static_cast<std::size_t>(board(i, j)) + 0x9e3779b9 + // NOLINT
                        (seed << 6) + (seed >> 2);                           // NOLINT
            }
        }

        return seed;
    }

    auto load() -> long {
        const auto rows{board.rows()};
        long weight{0};
        for (Eigen::Index j{0}; j < board.cols(); ++j) {
            for (Eigen::Index i{0}; i < rows; ++i) {
                if (board(i, j) == Round) {
                    weight += static_cast<long>(rows - i);
                }
            }
        }

        return weight;
    }

    auto print() -> void { std::cout << board << "\n"; };
};

int main() // NOLINT
{
    const auto lines = utils::read_lines_from_file("input.txt");
    const std::size_t total_c{1000000000};

    Board board(lines);
    std::unordered_map<std::size_t, std::size_t> records_map{};

    std::size_t cycle_length{0};
    std::size_t current{0};
    for (; current < total_c * 4; ++current) {
        board.tilt(static_cast<Direction>(current % 4));

        const auto hash{board.hash()};
        if (!records_map.contains(hash)) {
            records_map[hash] = current;
        } else {
            cycle_length = current - records_map[hash];
            break;
        }
    }
    auto rem_cycles = (total_c * 4 - 1 - current) % (cycle_length);
    for (std::size_t i{0}; i < rem_cycles; ++i) {
        board.tilt(static_cast<Direction>((current + i + 1) % 4));
    }
    std::cout << board.load() << "\n";
    return 0;
}