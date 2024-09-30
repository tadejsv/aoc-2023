// This runs relatively fase (sub 1 second), but I could probably add more optimizations in the path
// searching part - heuristics to early terminate some unfeasible paths.
#include <algorithm>
#include <Eigen/Dense>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <Eigen/src/Core/Matrix.h>
#include <Eigen/src/Core/util/Meta.h>

#include "utils/utils.h"

enum Tile : int {
    Path = 0,
    Forest = 1,
    Slope = 2,
};

enum Direction : int { Down = 0, Up = 1, Left = 2, Right = 3 };

using Node = std::pair<int, int>;  // row and column

using Neighbor = std::pair<std::size_t, int>;  // Node and distance

struct NeighborEqual {
    auto operator()(const Neighbor& lhs, const Neighbor& rhs) const -> bool {
        return lhs.first == rhs.first;
    }
};

struct NeighborHash {
    auto operator()(const Neighbor& neighbor) const -> std::size_t {
        return neighbor.first;
    }
};

struct TraverseIndex {
    Node current_pos;
    std::size_t last_node;
    int current_dist;
    Direction came_from;
};

struct PathIndex {
    unsigned long long visited{ 0 };
    std::size_t current_node;
    int current_dist;
};

int
main() {  // NOLINT
    const auto lines = utils::read_lines_from_file("input.txt");
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
                case '<':
                case '^':
                case 'v':
                    map(i, j) = Slope;
                    break;
                default:
                    throw "Unknown character";
            }
        }
    }

    const auto start{ std::chrono::high_resolution_clock::now() };

    // Prepare compressed graph. The graph will only contain "junction" nodes
    // and distances between them. For traversing the map we will be relying
    // on the assumptions that there that all visited cells are in the "interior"
    // of the map, so we don't need to check for boundaries - with the exception
    // of the final cell, which is in the last row.
    // We are also "ignoring" any possible dead ends - they do not change the
    // solution of the problem anyways.
    std::vector<Node> nodes{};
    std::unordered_map<std::size_t, std::unordered_set<Neighbor, NeighborHash, NeighborEqual>> graph{};
    std::queue<TraverseIndex> queue{};
    std::size_t final_node_ind{ 0 };
    nodes.emplace_back(0, 1);
    queue.emplace(Node{ 1, 1 }, 0, 1, Up);
    graph[0] = {};

    while (!queue.empty()) {
        const auto current_cell{ queue.front() };
        queue.pop();

        const auto& current_pos{ current_cell.current_pos };
        const auto came_from{ current_cell.came_from };
        auto last_node{ current_cell.last_node };
        auto current_dist{ current_cell.current_dist };

        // We have reached final node
        if (current_pos.first == map.rows() - 1) {
            nodes.push_back(current_pos);
            final_node_ind = nodes.size() - 1;
            graph[last_node].emplace(final_node_ind, current_dist);
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

        // If the cell is a node
        if (left_ok + rigt_ok + up_ok + down_ok > 1) {
            const auto curr_node_iter = std::find(nodes.begin(), nodes.end(), current_pos);
            const bool node_exists{ curr_node_iter != nodes.end() };

            // will be correct even if node does not exist
            const auto curr_node_ind{ static_cast<std::size_t>(curr_node_iter - nodes.begin()) };

            graph[last_node].emplace(curr_node_ind, current_dist);
            if (node_exists) {
                graph[curr_node_ind].emplace(last_node, current_dist);
                continue;
            }

            nodes.push_back(current_pos);
            graph[curr_node_ind] = {};
            graph[curr_node_ind].emplace(last_node, current_dist);

            last_node = curr_node_ind;
            current_dist = 0;
        }

        Node next_node;
        next_node = Node{ current_pos.first, current_pos.second - 1 };
        if (static_cast<bool>(left_ok)) {
            queue.emplace(next_node, last_node, current_dist + 1, Right);
        }
        next_node = Node{ current_pos.first, current_pos.second + 1 };
        if (static_cast<bool>(rigt_ok)) {
            queue.emplace(next_node, last_node, current_dist + 1, Left);
        }
        next_node = Node{ current_pos.first - 1, current_pos.second };
        if (static_cast<bool>(up_ok)) {
            queue.emplace(next_node, last_node, current_dist + 1, Down);
        }
        next_node = Node{ current_pos.first + 1, current_pos.second };
        if (static_cast<bool>(down_ok)) {
            queue.emplace(next_node, last_node, current_dist + 1, Up);
        }
    }

    // Use the compressed graph to get all possible paths from start to end
    std::queue<PathIndex> path_queue{};
    path_queue.emplace(1, 0, 0);

    int max_len{ 0 };
    PathIndex max_path{};
    while (!path_queue.empty()) {
        const auto current_cell{ path_queue.front() };
        path_queue.pop();
        if (current_cell.current_node == final_node_ind) {
            max_len = std::max(max_len, current_cell.current_dist);
            if (max_len == current_cell.current_dist) {
                max_path = current_cell;
            }
            continue;
        }

        for (const auto& neighbor : graph[current_cell.current_node]) {
            if ((current_cell.visited & (1ULL << (neighbor.first + 1))) == 0) {
                const auto visited{ current_cell.visited | (1ULL << (neighbor.first + 1)) };
                path_queue.emplace(visited, neighbor.first, current_cell.current_dist + neighbor.second);
            }
        }
    }
    const auto end{ std::chrono::high_resolution_clock::now() };
    auto ms_int = duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << max_len << "\n";
    std::cout << "Time taken: " << ms_int << "\n";

    return 0;
};
