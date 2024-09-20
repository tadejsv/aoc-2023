#include <algorithm>
#include <cstddef>
#include <Eigen/Dense>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "utils/utils.h"

using Eigen::MatrixXi;

enum Pipe { straight, vertical, l_bend, rev_l_bend, f_bend, rev_f_bend, ground, start };

// A convention for each join type that tells where the next connection should be - the
// opposite of where the previous connection came from
// F bend:     going +x is forward, -y is back
// Rev F bend: going +y is forward, -x is back
// Straight:   going +x is forward, -x is back
// Vertical:   going +y is forward, -y is back
// L bend:     going +x is forward, +y is back
// Rev L bend: going +y is forward, -x is back
enum Direction { back, forward, broken };

struct Point {
    Eigen::Index y;
    Eigen::Index x;
};

struct Move {
    Point p;
    Direction d;
};

auto
begin(const Point& start_pos, const Eigen::MatrixXi& grid) -> Move {
    if (start_pos.x > 0) {
        const auto val = grid(start_pos.y, start_pos.x - 1);
        if (val == straight || val == f_bend || val == l_bend) {
            return Move{ { start_pos.y, start_pos.x - 1 }, back };
        }
    }
    if (start_pos.y > 0) {
        const auto val = grid(start_pos.y - 1, start_pos.x);
        if (val == vertical || val == rev_f_bend) {
            return Move{ { start_pos.y - 1, start_pos.x }, back };
        }
        if (val == f_bend) {
            return Move{ { start_pos.y - 1, start_pos.x }, forward };
        }
    }
    if (start_pos.x < grid.cols() - 1) {
        const auto val = grid(start_pos.y, start_pos.x + 1);
        if (val == straight || val == rev_f_bend || val == rev_l_bend) {
            return Move{ { start_pos.y, start_pos.x + 1 }, forward };
        }
    }
    if (start_pos.y < grid.rows() - 1) {
        const auto val = grid(start_pos.y + 1, start_pos.x);
        if (val == vertical || val == l_bend) {
            return Move{ { start_pos.y + 1, start_pos.x }, forward };
        }

        if (val == rev_l_bend) {
            return Move{ { start_pos.y + 1, start_pos.x }, back };
        }
    }

    return Move{ start_pos, broken };
};

auto
next_back(const Move& move, const Eigen::MatrixXi& grid) -> Move {
    const auto val = grid(move.p.y, move.p.x);
    int nval{ 0 };
    switch (val) {
        case vertical:
        case l_bend:
            nval = grid(move.p.y - 1, move.p.x);
            if (nval == vertical || nval == rev_f_bend) {
                return Move{ { move.p.y - 1, move.p.x }, back };
            }
            if (nval == f_bend) {
                return Move{ { move.p.y - 1, move.p.x }, forward };
            }

            return Move{ { move.p.y - 1, move.p.x }, broken };
            break;
        case straight:
        case rev_l_bend:
        case rev_f_bend:
            return Move{ { move.p.y, move.p.x - 1 }, back };
            break;
        case f_bend:
            nval = grid(move.p.y + 1, move.p.x);
            if (nval == vertical || nval == l_bend) {
                return Move{ { move.p.y + 1, move.p.x }, forward };
            }
            if (nval == rev_l_bend) {
                return Move{ { move.p.y + 1, move.p.x }, back };
            }

            return Move{ { move.p.y + 1, move.p.x }, broken };
            break;
        default:
            return Move{ move.p, broken };
    };
};

auto
next_forward(const Move& move, const Eigen::MatrixXi& grid) -> Move {
    const auto val = grid(move.p.y, move.p.x);
    int nval{ 0 };
    switch (val) {
        case f_bend:
        case straight:
        case l_bend:
            return Move{ { move.p.y, move.p.x + 1 }, forward };
            break;
        case vertical:
        case rev_f_bend:
            nval = grid(move.p.y + 1, move.p.x);
            if (nval == vertical || nval == l_bend) {
                return Move{ { move.p.y + 1, move.p.x }, forward };
            }
            if (nval == rev_l_bend) {
                return Move{ { move.p.y + 1, move.p.x }, back };
            }

            return Move{ { move.p.y + 1, move.p.x }, broken };
            break;
        case rev_l_bend:
            nval = grid(move.p.y - 1, move.p.x);
            if (nval == vertical || nval == rev_f_bend) {
                return Move{ { move.p.y - 1, move.p.x }, back };
            }
            if (nval == f_bend) {
                return Move{ { move.p.y - 1, move.p.x }, forward };
            }

            return Move{ { move.p.y - 1, move.p.x }, broken };
            break;
        default:
            return Move{ move.p, broken };
    }
};

auto
next(const Move& move, const Eigen::MatrixXi& grid) -> Move {
    if (move.d == back) {
        return next_back(move, grid);
    }
    if (move.d == forward) {
        return next_forward(move, grid);
    }

    return Move{ move.p, broken };
}

int
main() {  // NOLINT
    const auto lines = utils::read_lines_from_file("input.txt");

    const Eigen::Index height{ static_cast<Eigen::Index>(lines.size()) };
    const Eigen::Index width{ static_cast<Eigen::Index>(lines.at(0).length()) };

    Point start_pos{ -1, -1 };
    MatrixXi grid = MatrixXi::Zero(height, width);
    MatrixXi loop = MatrixXi::Zero(height, width);
    for (Eigen::Index i{ 0 }; i < height; ++i) {
        const auto& line{ lines.at(static_cast<std::size_t>(i)) };
        for (Eigen::Index j{ 0 }; j < width; ++j) {
            switch (line.at(static_cast<std::size_t>(j))) {
                case '.':
                    grid(i, j) = ground;
                    break;
                case '-':
                    grid(i, j) = straight;
                    break;
                case '|':
                    grid(i, j) = vertical;
                    break;
                case 'F':
                    grid(i, j) = f_bend;
                    break;
                case '7':
                    grid(i, j) = rev_f_bend;
                    break;
                case 'L':
                    grid(i, j) = l_bend;
                    break;
                case 'J':
                    grid(i, j) = rev_l_bend;
                    break;
                case 'S':
                    grid(i, j) = start;
                    start_pos = { i, j };
                    break;
                default:
                    std::cout << "Uknown symbol " << line.at(static_cast<std::size_t>(j)) << i
                              << " " << j << "\n";
            }
        }
    }

    if (start_pos.x < 0) {
        std::cout << "Did not find start position";
        return 1;
    }

    std::cout << "Starting at " << start_pos.x << "," << start_pos.y << "\n";
    loop(start_pos.y, start_pos.x) = 1;

    auto cont{ begin(start_pos, grid) };
    if (cont.d == broken) {
        std::cout << "Nowhere to go from start";
        return 1;
    }

    int loop_length{ 1 };

    while (start_pos.x != cont.p.x || start_pos.y != cont.p.y) {
        loop(cont.p.y, cont.p.x) = 1;
        if (cont.d == broken) {
            std::cout << "Oh no! Path is broken!";
            return 1;
        }
        cont = next(cont, grid);
        loop_length++;

        // Safety valve
        if (loop_length > height * width) {
            break;
        }
    }

    // Vertical right-down pass
    int total_enclosed{ 0 };
    Eigen::Index x{ 0 };
    Eigen::Index y{ 0 };
    const auto mhw{ std::min(height, width) };
    for (Eigen::Index i{ 0 }; i < width + height; i++) {
        int crossings{ 0 };
        Eigen::Index maxj{ 0 };
        if (i < width) {
            maxj = std::min(i, mhw);
        } else {
            maxj = std::min(height + width - i - 1, mhw);
        }
        for (Eigen::Index j{ 0 }; j < maxj; j++) {
            // Get x, y coordinates
            if (i < width) {
                x = width - 1 - i + j;
                y = j;
            } else {
                x = j;
                y = i + 1 - width + j;
            }

            if (loop(y, x) == 1) {
                const auto pipe{ grid(y, x) };
                // If path goes through these pipe types, it only "skims" the loop
                if (pipe != l_bend && pipe != rev_f_bend) {
                    crossings++;
                }
            } else if (crossings % 2 == 1) {
                total_enclosed += 1;
            }
        }
    }

    std::cout << total_enclosed << "\n";
}