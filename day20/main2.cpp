// I'll be honest - the solution to this task was "inspired" by some solutions I found
// online :)
#include <iostream>
#include <memory>
#include <numeric>
#include <queue>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "utils/utils.h"

enum Signal { Low, High };

struct Pulse {
    std::string from;
    std::string to;
    Signal signal{ Low };
};

class Node {
public:

    bool on{ false };
    std::vector<std::string> connections{};

    Node(const std::vector<std::string>& conns)
        : connections{ conns } {};

    virtual auto apply(const Pulse& pulse) -> std::vector<Pulse> = 0;

    virtual ~Node() = default;
    Node(const Node&) = default;
    auto operator=(const Node&) -> Node& = default;
    Node(Node&&) noexcept = default;
    auto operator=(Node&&) noexcept -> Node& = default;
};

class FlipFlop : public Node {
public:

    FlipFlop(const std::vector<std::string>& connections)
        : Node{ connections } {};

    auto apply(const Pulse& pulse) -> std::vector<Pulse> override {
        if (pulse.signal == High) {
            return {};
        }

        std::vector<Pulse> pulses{};
        pulses.reserve(connections.size());
        const auto signal = on ? Low : High;

        for (const auto& conn : connections) {
            pulses.emplace_back(pulse.to, conn, signal);
        }
        on = !on;

        return pulses;
    };
};

class Conjunction : public Node {
public:

    std::unordered_map<std::string, Signal> last_received{};
    Conjunction(const std::vector<std::string>& connections)
        : Node{ connections } {};

    auto apply(const Pulse& pulse) -> std::vector<Pulse> override {
        last_received[pulse.from] = pulse.signal;
        Signal signal{ Low };
        for (const auto& [nn, value] : last_received) {
            if (value == Low) {
                signal = High;
                break;
            }
        }

        std::vector<Pulse> pulses{};
        pulses.reserve(connections.size());
        for (const auto& conn : connections) {
            pulses.emplace_back(pulse.to, conn, signal);
        }

        return pulses;
    };
};

class Broadcaster : public Node {
public:

    Broadcaster(const std::vector<std::string>& connections)
        : Node{ connections } {};

    auto apply(const Pulse& pulse) -> std::vector<Pulse> override {
        std::vector<Pulse> pulses{};
        pulses.reserve(connections.size());
        for (const auto& conn : connections) {
            pulses.emplace_back(pulse.to, conn, pulse.signal);
        }

        return pulses;
    };
};

constexpr int NUM_LOOPS{ 5000 };

int
main()  // NOLINT
{
    const auto lines = utils::read_lines_from_file("input.txt");

    std::unordered_set<std::string> inverters{};
    std::unordered_map<std::string, std::unique_ptr<Node>> nodes{};
    for (const auto& line : lines) {
        const auto parts{ utils::split_string(line, '-') };
        const auto identifier{ parts[0][0] };
        auto name{ parts[0].substr(1, parts[0].size() - 2) };

        const auto conns_strs{ utils::split_string(parts[1].substr(1, parts[1].size() - 1), ',') };
        std::vector<std::string> conns{};
        conns.reserve(conns_strs.size());
        for (const auto& conn_str : conns_strs) {
            conns.push_back(conn_str.substr(1, conn_str.size() - 1));
        }

        if (identifier == '%') {
            nodes[name] = std::make_unique<FlipFlop>(conns);
        } else if (identifier == '&') {
            nodes[name] = std::make_unique<Conjunction>(conns);
            inverters.insert(name);
        } else {
            name = parts[0].substr(0, parts[0].size() - 1);
            nodes[name] = std::make_unique<Broadcaster>(conns);
        }
    }

    // Second pass for inverter modules
    for (const auto& [name, node] : nodes) {
        for (const auto& conn_node : node->connections) {
            if (inverters.contains(conn_node)) {
                auto* const node = dynamic_cast<Conjunction*>(nodes[conn_node].get());
                node->last_received[name] = Low;
            }
        }
    }

    // Get the node (inverter) connected to rx
    const std::string TARGET{ "rx" };
    std::string rx_feed{};
    for (const auto& [name, node] : nodes) {
        for (const auto& conn_node : node->connections) {
            if (conn_node == TARGET) {
                rx_feed = name;
            }
        }
    }

    // Get the nodes (inverters) connected to rx feed
    std::unordered_map<std::string, int> final_inverter_periods{};
    for (const auto& [name, node] : nodes) {
        for (const auto& conn_node : node->connections) {
            if (conn_node == rx_feed) {
                final_inverter_periods[name] = -1;
            }
        }
    }

    std::queue<Pulse> pulses{};
    for (int i{ 0 }; i < NUM_LOOPS; ++i) {
        pulses.emplace("button", "broadcaster", Low);
        while (!pulses.empty()) {
            const auto& pulse = pulses.front();

            if (pulse.signal == High && final_inverter_periods.contains(pulse.from)
                && final_inverter_periods[pulse.from] < 0) {
                final_inverter_periods[pulse.from] = i + 1;
            }

            if (!nodes.contains(pulse.to)) {
                pulses.pop();
                continue;
            }

            for (const auto& pulse : nodes[pulse.to]->apply(pulse)) {
                pulses.push(pulse);
            };

            pulses.pop();
        }
    }

    long long lcm{ 1 };
    for (const auto& [k, v] : final_inverter_periods) {
        lcm = std::lcm(lcm, v);
        std::cout << k << " " << v << "\n";
    }
    std::cout << lcm << "\n";

    return 0;
};