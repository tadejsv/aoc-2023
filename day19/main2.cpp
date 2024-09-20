#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <__fwd/get.h>

#include "utils/utils.h"

enum Operation { less, more, last };

enum Category { x = 0, m = 1, a = 2, s = 3 };

// Representation of a half open interval [upper, lower)
class Interval {
public:

    long long upper{ 4001 };  // NOLINT
    long long lower{ 1 };

    auto cut(Operation opr, long long threshold) -> void {
        if (opr == less) {
            upper = std::max(lower, std::min(threshold, upper));
        } else {  // Assumed to be more
            lower = std::min(upper, std::max(threshold, lower));
        }
    }

    [[nodiscard]] auto empty() const -> bool {
        return lower >= upper;
    }

    [[nodiscard]] auto size() const -> long long {
        return upper - lower;
    }
};

class Item {
public:

    std::array<Interval, 4> intervals{};

    [[nodiscard]] auto empty() const -> bool {
        return intervals[x].empty() || intervals[m].empty() || intervals[a].empty()
               || intervals[s].empty();
    }

    [[nodiscard]] auto size() const -> long long {
        return intervals[x].size() * intervals[m].size() * intervals[a].size() * intervals[s].size();
    }
};

class Rule {
public:

    Category cat;
    Operation op;
    long long threshold;
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

            op = (input[1] == '<') ? less : more;

            const auto parts{ utils::split_string(input.substr(2), ':') };

            threshold = std::stoll(parts[0]);
            next = parts[1];
        } else {
            op = last;
            cat = x;
            threshold = 0;
            next = input;
        }
    };

    [[nodiscard]] auto apply(const Item& item) const -> std::tuple<Item, Item> {
        if (op == last) {
            return std::make_tuple(item, item);
        }

        Item accept{ item };
        Item reject{ item };
        Interval& accept_val = accept.intervals[cat];  // NOLINT
        Interval& reject_val = reject.intervals[cat];  // NOLINT

        if (op == less) {
            accept_val.cut(less, threshold);
            reject_val.cut(more, threshold);
        } else {  // Has to be more in this case
            accept_val.cut(more, threshold + 1);
            reject_val.cut(less, threshold + 1);
        }
        return std::make_tuple(accept, reject);
    };
};

struct QueueItem {
    Item item;
    std::string wfn;
    std::size_t rule_ind;
};

int
main()  // NOLINT
{
    const auto lines = utils::read_lines_from_file("input.txt");

    std::unordered_map<std::string, std::vector<Rule>> workflows{};

    for (const auto& line : lines) {
        if (line.empty()) {
            break;
        }
        const auto parts{ utils::split_string(line, '{') };
        const auto rules_str{ utils::split_string(parts[1].substr(0, parts[1].size() - 1), ',') };

        std::vector<Rule> rules{};
        rules.reserve(rules_str.size());
        for (const auto& rule_str : rules_str) {
            rules.emplace_back(rule_str);
        }
        workflows[parts[0]] = rules;
    }

    const auto start{ std::chrono::high_resolution_clock::now() };
    std::vector<Item> accepted{};
    std::queue<QueueItem> queue{};
    queue.emplace(Item{}, "in", 0);

    while (!queue.empty()) {
        const auto& qitem{ queue.front() };
        const auto& rule{ workflows[qitem.wfn][qitem.rule_ind] };
        const auto& item{ qitem.item };

        if (rule.op == last) {
            if (rule.next == "A") {
                accepted.push_back(item);
            } else if (rule.next != "R") {
                queue.emplace(item, rule.next, 0);
            }
        } else {
            const auto [good, bad] = rule.apply(item);
            if (!good.empty()) {
                if (rule.next == "A") {
                    accepted.push_back(good);
                } else if (rule.next != "R") {
                    queue.emplace(good, rule.next, 0);
                }
            }
            if (!bad.empty()) {
                queue.emplace(bad, qitem.wfn, qitem.rule_ind + 1);
            }
        }

        queue.pop();
    }

    long long sum{ 0 };
    for (const auto& item : accepted) {
        sum += item.size();
    }
    const auto end{ std::chrono::high_resolution_clock::now() };
    auto ms_int = duration_cast<std::chrono::microseconds>(end - start);

    std::cout << sum << "\n";
    std::cout << "Time taken: " << ms_int << " \n";

    return 0;
};
