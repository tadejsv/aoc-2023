#include <algorithm>
#include <Eigen/Dense>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

#include <Eigen/src/Core/Matrix.h>
#include <Eigen/src/Core/util/Meta.h>

#include "utils/utils.h"

enum Tile : int {
    Path = 0,
    Forest = 1,
    SlopeUp = 2,
    SlopeDown = 3,
    SlopeLeft = 4,
    SlopeRight = 5,
};

enum Direction : int { Down = 0, Up = 1, Left = 2, Right = 3 };

using Node = std::pair<int, int>;

struct NodeHash {
    auto operator()(const Node& node) const -> std::size_t {
        return static_cast<std::size_t>((node.first << 12) | (node.second));  // NOLINT
    }
};

struct Neighbor {
    Node node;
    int dist{ 0 };
};

struct TraverseIndex {
    Node current_pos;
    Node last_junction;
    int current_dist;
    Direction came_from;
};

struct PathIndex {
    Node current_pos;
    int current_dist;
};

int
main() {  // NOLINT
    const auto lines = utils::read_lines_from_file("test_input.txt");
    Eigen::Index n_rows{ static_cast<Eigen::Index>(lines.size()) };
    Eigen::Index n_cols{ static_cast<Eigen::Index>(lines[0].size()) };

    Eigen::MatrixXi map{ Eigen::MatrixXi::Zero(n_rows, n_cols) };

    // Parse input
    for (Eigen::Index i{ 0 }; i < n_rows; ++i) {
        for (Eigen::Index j{ 0 }; j < n_cols; ++j) {
            switch (lines[static_cast<std::size_t>(i)][static_cast<std::size_t>(j)]) {
                case '#':
                    map(i, j) = Forest;
                    break;
                case '.':
                    map(i, j) = Path;
                    break;
                case '>':
                    map(i, j) = SlopeRight;
                    break;
                case '<':
                    map(i, j) = SlopeLeft;
                    break;
                case '^':
                    map(i, j) = SlopeUp;
                    break;
                case 'v':
                    map(i, j) = SlopeDown;
                    break;
                default:
                    throw "Unknown character";
            }
        }
    }

    // Prepare compressed graph. The graph will only contain "junction" nodes
    // and distances between them. For traversing the map we will be relying
    // on the assumptions that all visited cells are in the "interior" of the
    // map, so we don't need to check for boundaries - with the exception of the
    // final cell, which is in the last row.
    // We are also "ignoring" any possible dead ends - they do not change the
    // solution of the problem anyways.
    // For the slides, we assume they always appear either in the direction in
    // which the user is already going, or in the opposite direction - and
    // thus stopping the path. Strange-but-valid (slide left when going up)
    // combinations are assumed to not exist.
    std::unordered_map<Node, std::vector<Neighbor>, NodeHash> graph{};
    std::queue<TraverseIndex> queue{};
    queue.emplace(Node{ 1, 1 }, Node{ 0, 1 }, 1, Up);
    graph[Node{ 0, 1 }] = std::vector<Neighbor>{};

    while (!queue.empty()) {
        const auto current_cell{ queue.front() };
        queue.pop();

        const auto& current_pos{ current_cell.current_pos };
        const auto came_from{ current_cell.came_from };
        auto last_junction{ current_cell.last_junction };
        auto current_dist{ current_cell.current_dist };

        // We are in end node
        if (current_pos.first == map.rows() - 1) {
            graph[last_junction].emplace_back(current_pos, current_dist);
            continue;
        }

        int left_ok{ static_cast<int>(
            map(current_pos.first, current_pos.second - 1) != Forest && came_from != Left
        ) };
        int rigt_ok{ static_cast<int>(
            map(current_pos.first, current_pos.second + 1) != Forest && came_from != Right
        ) };
        int up_ok{ static_cast<int>(
            map(current_pos.first - 1, current_pos.second) != Forest && came_from != Up
        ) };
        int down_ok{ static_cast<int>(
            map(current_pos.first + 1, current_pos.second) != Forest && came_from != Down
        ) };

        bool is_node{ left_ok + rigt_ok + up_ok + down_ok > 1 };

        if (is_node) {
            graph[last_junction].emplace_back(current_pos, current_dist);
            if (graph.contains(current_pos)) {
                continue;
            }
            graph[current_pos] = std::vector<Neighbor>{};
            last_junction = current_pos;
            current_dist = 0;
        }

        Node next_node;
        next_node = Node{ current_pos.first, current_pos.second - 1 };
        if (static_cast<bool>(left_ok) && map(next_node.first, next_node.second) != SlopeRight) {
            queue.emplace(next_node, last_junction, current_dist + 1, Right);
        }
        next_node = Node{ current_pos.first, current_pos.second + 1 };
        if (static_cast<bool>(rigt_ok) && map(next_node.first, next_node.second) != SlopeLeft) {
            queue.emplace(next_node, last_junction, current_dist + 1, Left);
        }
        next_node = Node{ current_pos.first - 1, current_pos.second };
        if (static_cast<bool>(up_ok) && map(next_node.first, next_node.second) != SlopeDown) {
            queue.emplace(next_node, last_junction, current_dist + 1, Down);
        }
        next_node = Node{ current_pos.first + 1, current_pos.second };
        if (static_cast<bool>(down_ok) && map(next_node.first, next_node.second) != SlopeUp) {
            queue.emplace(next_node, last_junction, current_dist + 1, Up);
        }
    }

    // Use the compressed graph to get all possible paths from start to end
    std::queue<PathIndex> path_queue{};
    path_queue.emplace(Node{ 0, 1 }, 0);

    int max_len{ 0 };
    while (!path_queue.empty()) {
        const auto current_cell{ path_queue.front() };
        path_queue.pop();
        if (current_cell.current_pos.first == map.rows() - 1) {
            max_len = std::max(max_len, current_cell.current_dist);
            continue;
        }

        for (const auto& neighbor : graph[current_cell.current_pos]) {
            path_queue.emplace(neighbor.node, current_cell.current_dist + neighbor.dist);
        }
    }

    std::cout << max_len << "\n";
    return 0;
};
