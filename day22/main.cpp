#include <algorithm>
#include <Eigen/Dense>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "utils/utils.h"

struct Position {
    int x, y, z;
};

struct Brick {
    Position start, end;
};

struct GraphNode {
    std::vector<std::size_t> supports{};
    std::vector<std::size_t> supported_by{};
};

auto
compare_bricks(const Brick& brick1, const Brick& brick2) -> bool {
    return brick1.start.z < brick2.start.z;
}

int
main() {  // NOLINT
    const auto lines = utils::read_lines_from_file("input.txt");
    std::vector<Brick> bricks{};

    int max_x{ 0 };
    int max_y{ 0 };
    for (const auto& line : lines) {
        const auto parts{ utils::split_string(line, '~') };
        const auto start_parts{ utils::split_string(parts[0], ',') };
        const auto end_parts{ utils::split_string(parts[1], ',') };

        const Position start{
            std::stoi(start_parts[0]),
            std::stoi(start_parts[1]),
            std::stoi(start_parts[2]),
        };
        const Position end{
            std::stoi(end_parts[0]),
            std::stoi(end_parts[1]),
            std::stoi(end_parts[2]),
        };
        bricks.emplace_back(start, end);

        // Assume that coords in 2nd endpoint always >= than coords in 1st endpoint
        max_x = std::max(max_x, end.x);
        max_y = std::max(max_y, end.y);
    }

    const auto start{ std::chrono::high_resolution_clock::now() };

    // Let the bricks fall
    Eigen::MatrixXi max_height{ Eigen::MatrixXi::Ones(max_x + 1, max_y + 1) };
    std::sort(bricks.begin(), bricks.end(), compare_bricks);

    for (std::size_t brick_ind{ 0 }; brick_ind < bricks.size(); ++brick_ind) {
        Brick& brick{ bricks[brick_ind] };
        Position& start{ brick.start };
        Position& end{ brick.end };
        const int brick_height{ 1 + end.z - start.z };

        int max_h{ 0 };
        if (start.x < end.x) {
            max_h = max_height.col(start.y).segment(start.x, end.x - start.x + 1).maxCoeff();
        } else {  // y is different
            max_h = max_height.row(start.x).segment(start.y, end.y - start.y + 1).maxCoeff();
        }

        const auto new_max_h{ max_h + brick_height };

        start.z = max_h;
        end.z = max_h + brick_height - 1;

        if (start.x < end.x) {
            max_height.col(start.y).segment(start.x, end.x - start.x + 1).setConstant(new_max_h);
        } else {  // y is different
            max_height.row(start.x).segment(start.y, end.y - start.y + 1).setConstant(new_max_h);
        }
    }

    // Deduce the support graph
    std::unordered_map<int, std::vector<std::size_t>> height_map{};
    for (std::size_t brick_ind{ 0 }; brick_ind < bricks.size(); ++brick_ind) {
        const Brick& brick{ bricks[brick_ind] };
        const auto max_h{ brick.end.z };
        if (!height_map.contains(max_h)) {
            height_map[max_h] = std::vector<std::size_t>{ brick_ind };
        } else {
            height_map[max_h].push_back(brick_ind);
        }
    }

    std::vector<GraphNode> support_graph(bricks.size());
    for (std::size_t brick_ind{ 0 }; brick_ind < bricks.size(); ++brick_ind) {
        const Brick& brick{ bricks[brick_ind] };

        // Find out who supports this brick
        if (!height_map.contains(brick.start.z - 1)) {
            continue;
        }

        std::vector<std::size_t> potential_supports{ height_map[brick.start.z - 1] };
        for (const auto potential_support : potential_supports) {
            const Brick& pbrick{ bricks[potential_support] };
            bool support = brick.start.y <= pbrick.end.y && pbrick.start.y <= brick.end.y
                           && brick.start.x <= pbrick.end.x && pbrick.start.x <= brick.end.x;

            if (support) {
                support_graph[brick_ind].supported_by.push_back(potential_support);
                support_graph[potential_support].supports.push_back(brick_ind);
            }
        }
    }

    int cool_bricks{ 0 };
    for (std::size_t brick_ind{ 0 }; brick_ind < bricks.size(); ++brick_ind) {
        bool can_disintegrate{ true };
        const auto supported_bricks{ support_graph[brick_ind].supports };
        for (const auto supported_brick : supported_bricks) {
            if (support_graph[supported_brick].supported_by.size() == 1) {
                can_disintegrate = false;
                break;
            }
        }

        if (can_disintegrate) {
            cool_bricks += 1;
        }
    }

    const auto end{ std::chrono::high_resolution_clock::now() };
    auto ms_int = duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << cool_bricks << "\n";
    std::cout << "Time taken: " << ms_int << "\n";

    return 0;
};
