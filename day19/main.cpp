#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "utils/utils.h"

enum Operation { less, more, last };

enum Category { x, m, a, s };

class Item {
public:

    int x;
    int m;
    int a;
    int s;

    Item(std::string_view input) {
        const auto& parts{ utils::split_string(input.substr(1, input.size() - 1), ',') };
        for (const auto& part : parts) {
            int val{ std::stoi(part.substr(2)) };
            switch (part[0]) {
                case 'x':
                    x = val;
                    break;
                case 'm':
                    m = val;
                    break;
                case 'a':
                    a = val;
                    break;
                case 's':
                    s = val;
                    break;
                default:
                    throw std::invalid_argument("Invalid character for Category enum");
            }
        }
    }
};

class Rule {
public:

    Category cat;
    Operation op;
    int threshold;
    std::string next;

    Rule(std::string_view input) {
        if (input[1] == '<' || input[1] == '>') {
            switch (input[0]) {
                case 'x':
                    cat = x;
                    break;
                case 'm':
                    cat = m;
                    break;
                case 'a':
                    cat = a;
                    break;
                case 's':
                    cat = s;
                    break;
                default:
                    throw std::invalid_argument("Invalid character for Category enum");
            }

            if (input[1] == '<') {
                op = less;
            } else {
                op = more;
            }

            const auto parts{ utils::split_string(input.substr(2), ':') };

            threshold = std::stoi(parts[0]);
            next = parts[1];
        } else {
            op = last;
            cat = x;
            threshold = 0;
            next = input;
        }
    };

    [[nodiscard]] auto apply(const Item& item) const -> bool {
        if (op == last) {
            return true;
        }

        int val{ 0 };
        switch (cat) {
            case x:
                val = item.x;
                break;
            case m:
                val = item.m;
                break;
            case a:
                val = item.a;
                break;
            case s:
                val = item.s;
                break;
        }

        if (op == less) {
            return val < threshold;
        }
        return val > threshold;
    };
};

int
main()  // NOLINT
{
    const auto lines = utils::read_lines_from_file("input.txt");

    const auto break_line = std::find_if(lines.begin(), lines.end(), [](const std::string& str) {
        return str.empty();
    });
    const auto break_ind{ static_cast<std::size_t>(std::distance(lines.begin(), break_line)) };

    std::unordered_map<std::string, std::vector<Rule>> workflows{};

    for (std::size_t i{ 0 }; i < break_ind; ++i) {
        const auto parts{ utils::split_string(lines[i], '{') };
        const auto rules_str{ utils::split_string(parts[1].substr(0, parts[1].size() - 1), ',') };

        std::vector<Rule> rules{};
        rules.reserve(rules_str.size());
        for (const auto& rule_str : rules_str) {
            rules.emplace_back(rule_str);
        }
        workflows[parts[0]] = rules;
    }

    std::vector<Item> items{};
    for (std::size_t i{ break_ind + 1 }; i < lines.size(); ++i) {
        items.emplace_back(lines[i]);
    }

    std::vector<Item> accepted{};
    for (const auto& item : items) {
        std::string wfn{ "in" };

        while (wfn != "A" && wfn != "R") {
            const auto& workflow{ workflows[wfn] };
            for (const auto& rule : workflow) {
                auto res = rule.apply(item);
                if (res) {
                    wfn = rule.next;
                    break;
                }
            }
        }

        if (wfn == "A") {
            accepted.push_back(item);
        }
    }

    int sum{ 0 };
    for (const auto& item : accepted) {
        sum += item.x + item.a + item.s + item.m;
    }
    std::cout << sum << "\n";

    return 0;
};
