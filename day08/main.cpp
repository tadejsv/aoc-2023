#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include "utils/utils.h"

constexpr std::string_view START_NODE{ "AAA" };
constexpr std::string_view END_NODE{ "ZZZ" };

constexpr char LEFT{ 'L' };
constexpr char RIGHT{ 'R' };
constexpr std::size_t L_IND{ 0 };
constexpr std::size_t R_IND{ 1 };

struct Graph {
    std::unordered_map<std::string, std::pair<std::string, std::string>> network;
    std::string walk_pattern;

    auto walk(std::string_view start_node, std::string_view end_node)  // NOLINT
        -> void {
        std::size_t current_step{ 0 };
        std::string current_node{ start_node };
        long num_steps{ 0 };

        while (current_node != end_node) {
            switch (walk_pattern.at(current_step)) {
                case LEFT:
                    current_node = std::get<L_IND>(network[current_node]);
                    break;
                case RIGHT:
                    current_node = std::get<R_IND>(network[current_node]);
                    break;
                default:
                    break;
            }

            ++current_step;
            ++num_steps;
            if (current_step >= walk_pattern.size()) {
                current_step = static_cast<std::size_t>(0);
            }
        };

        std::cout << num_steps;
    };
};

int
main() {  // NOLINT
    const auto lines = utils::read_lines_from_file("input.txt");

    Graph graph{};

    graph.walk_pattern = lines.at(0);

    for (std::size_t i{ 2 }; i < lines.size(); i++) {
        std::string_view line{ lines.at(i) };

        auto node_connections = utils::split_string(line, '=');
        auto node_name{ node_connections.at(0).substr(0, 3) };

        auto conns = utils::split_string(node_connections.at(1), ',');
        auto conn1 = conns.at(0).substr(2);
        auto conn2 = conns.at(1).substr(1, 3);

        graph.network[node_name] = std::pair(conn1, conn2);
    }

    graph.walk(START_NODE, END_NODE);

    return 0;
}