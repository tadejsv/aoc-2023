// This code uses an implementation of Stoer-Wagner algorithm for finding the minimum cut of the
// graph. Even though I have tried to optimise it a bit (all string keys have been converted to
// ints), it is still quite inefficient - it clocks in at about 1 minute.
#include <iostream>
#include <ranges>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "utils/utils.h"

using Graph = std::unordered_map<int, std::unordered_map<int, int>>;

auto
stringToInt(const std::string& str) -> int {
    int result = 0;
    int weight = 1;  // Start with a weight of 1

    for (char chr : str) {
        result += static_cast<int>(chr) * weight;
        weight *= 31;  // NOLINT
    }
    return result;
}

auto
min_cut_phase(Graph& graph, std::unordered_map<int, int>& sizes) -> std::pair<int, int> {
    auto key_view = std::views::keys(graph);
    std::unordered_set<int> remaining{ key_view.begin(), key_view.end() };

    int last_size{ 0 };
    int max_cut{ 0 };

    int last_node{ *remaining.begin() };
    int last2_node{};
    remaining.erase(last_node);

    while (!remaining.empty()) {
        int max_node{ 0 };
        int max_conns{ 0 };

        // go over all remaining keys, get the max
        for (const auto& key : remaining) {
            int conns{ 0 };

            for (const auto& conn : graph[key]) {
                if (!remaining.contains(conn.first)) {
                    conns += conn.second;
                }
            }

            if (conns > max_conns) {
                max_conns = conns;
                max_node = key;
            }
        }
        remaining.erase(max_node);

        last2_node = last_node;
        last_node = max_node;
        max_cut = max_conns;
    }
    last_size = sizes[last_node];

    // modify graph by joining the last two nodes
    std::unordered_set<int> common_neighbors;
    for (const auto& neighbor : graph[last_node]) {
        common_neighbors.insert(neighbor.first);
    }
    for (const auto& neighbor : graph[last2_node]) {
        common_neighbors.insert(neighbor.first);
    }
    common_neighbors.erase(last_node);
    common_neighbors.erase(last2_node);
    for (const auto& neighbor : common_neighbors) {
        const auto new_cost{ graph[neighbor][last_node] + graph[neighbor][last2_node] };
        graph[neighbor][last_node] = new_cost;
        graph[last_node][neighbor] = new_cost;
        graph[neighbor].erase(last2_node);
    }

    graph.erase(last2_node);
    graph[last_node].erase(last2_node);
    sizes[last_node] += sizes[last2_node];

    return std::make_pair(max_cut, last_size);
};

int
main() {  // NOLINT
    const auto lines = utils::read_lines_from_file("input.txt");

    Graph graph{};
    for (const auto& line : lines) {
        const auto& line_parts{ utils::split_string(line, ':') };
        const auto& node1{ stringToInt(line_parts[0]) };
        const auto& connections{ utils::split_string(utils::trim(line_parts[1]), ' ') };
        for (const auto& conn_str : connections) {
            const int conn{ stringToInt(conn_str) };
            if (!graph.contains(node1)) {
                graph[node1] = std::unordered_map<int, int>{};
            }
            if (!graph.contains(conn)) {
                graph[conn] = std::unordered_map<int, int>{};
            }

            graph[node1][conn] = 1;
            graph[conn][node1] = 1;
        }
    }

    std::unordered_map<int, int> sizes{};
    for (const auto& ele : graph) {
        sizes[ele.first] = 1;
    }

    int min_cut{ static_cast<int>(graph.size() * graph.size()) };
    int partition_size{ 0 };
    int full_size{ static_cast<int>(graph.size()) };
    while (graph.size() > 1) {
        auto [mcp, gsize] = min_cut_phase(graph, sizes);
        if (mcp < min_cut) {
            min_cut = mcp;
            partition_size = gsize;
        }
    }

    std::cout << (full_size - partition_size) * partition_size << "\n";
    return 0;
};