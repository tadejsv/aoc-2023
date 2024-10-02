// This runs relatively fast (sub 1 second), but I could probably add more optimizations in the path
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

enum Tile : int { Path = 0, Forest = 1, Slope = 2 };

enum Direction : int { Down = 0, Up = 1, Left = 2, Right = 3 };

using Node = std::pair<int, int>;  // row and column

using Neighbor = std::pair<std::size_t, int>;  // Node ind and distance

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
    std::size_t last_node_ind;
    int dist;
    Direction came_from;
};

struct PathIndex {
    unsigned long visited{ 0 };
    std::size_t current_node_ind{ 0 };
    int dist{ 0 };
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

    for (; !queue.empty(); queue.pop()) {
        const auto current_cell{ queue.front() };
        const auto [cur_row, cur_col] = current_cell.current_pos;
        const auto came_from{ current_cell.came_from };
        auto last_node_ind{ current_cell.last_node_ind };
        auto dist{ current_cell.dist };

        // We have reached final node
        if (cur_row == map.rows() - 1) {
            nodes.emplace_back(cur_row, cur_col);
            final_node_ind = nodes.size() - 1;
            graph[last_node_ind].emplace(final_node_ind, dist);
            continue;
        }

        bool left_ok{ map(cur_row, cur_col - 1) != Forest && came_from != Left };
        bool right_ok{ map(cur_row, cur_col + 1) != Forest && came_from != Right };
        bool up_ok{ map(cur_row - 1, cur_col) != Forest && came_from != Up };
        bool down_ok{ map(cur_row + 1, cur_col) != Forest && came_from != Down };

        // If the cell is a node
        if (int(left_ok) + int(right_ok) + int(up_ok) + int(down_ok) > 1) {
            // index be correct even if node does not exist, as we will add it at the end
            const auto curr_node_iter = std::find(nodes.begin(), nodes.end(), current_cell.current_pos);
            const auto curr_node_ind{ static_cast<std::size_t>(curr_node_iter - nodes.begin()) };

            graph[last_node_ind].emplace(curr_node_ind, dist);
            if (curr_node_iter != nodes.end()) {  // node exists
                graph[curr_node_ind].emplace(last_node_ind, dist);
                continue;
            }

            nodes.push_back(current_cell.current_pos);
            graph[curr_node_ind] = {};
            graph[curr_node_ind].emplace(last_node_ind, dist);

            last_node_ind = curr_node_ind;
            dist = 0;
        }


        auto enqueue_node = [&](int d_row, int d_col, Direction came_from) {
            queue.emplace(Node{ cur_row + d_row, cur_col + d_col }, last_node_ind, dist + 1, came_from);
        };
        if (left_ok) {
            enqueue_node(0, -1, Right);
        }
        if (right_ok) {
            enqueue_node(0, 1, Left);
        }
        if (up_ok) {
            enqueue_node(-1, 0, Down);
        }
        if (down_ok) {
            enqueue_node(1, 0, Up);
        }
    }

    // Use the compressed graph to get all possible paths from start to end
    std::queue<PathIndex> path_queue{};
    path_queue.emplace(1, 0, 0);

    int max_len{ 0 };
    for (; !path_queue.empty(); path_queue.pop()) {
        const auto& [current_visited, current_node_ind, dist] = path_queue.front();
        if (current_node_ind == final_node_ind) {
            max_len = std::max(max_len, dist);
            continue;
        }

        for (const auto& [neighbor_node, neighbor_dist] : graph[current_node_ind]) {
            if ((current_visited & (1ULL << (neighbor_node + 1))) == 0) {
                const auto visited{ current_visited | (1ULL << (neighbor_node + 1)) };
                path_queue.emplace(visited, neighbor_node, dist + neighbor_dist);
            }
        }
    }
    const auto end{ std::chrono::high_resolution_clock::now() };
    auto ms_int = duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << max_len << "\n";
    std::cout << "Time taken: " << ms_int << "\n";

    return 0;
};
