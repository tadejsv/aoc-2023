#include "day7.h"
#include <algorithm>
#include <array>
#include <cstddef>
#include <string_view>
#include <unordered_map>
#include <vector>

static const std::unordered_map<char, int> card_strength = {
    {'A', 14}, {'K', 13}, {'Q', 12}, {'J', 11}, {'T', 10}, {'9', 9}, {'8', 8},
    {'7', 7},  {'6', 6},  {'5', 5},  {'4', 4},  {'3', 3},  {'2', 2}};

static const char JOKER{'J'};
static const std::unordered_map<char, int> jcard_strength = {
    {'A', 14}, {'K', 13}, {'Q', 12}, {'T', 10}, {'9', 9}, {'8', 8}, {'7', 7},
    {'6', 6},  {'5', 5},  {'4', 4},  {'3', 3},  {'2', 2}, {'J', 1}};

Hand::Hand(std::string_view hand) { // NOLINT(cppcoreguidelines-pro-type-member-init)
    std::unordered_map<char, int> counts_map;
    for (std::size_t i{0}; i < hand.size(); ++i) {
        characters.at(i) = hand[i];
        counts_map[hand[i]]++;
    }

    std::vector<int> counts;
    counts.reserve(counts_map.size());
    for (const auto &pair : counts_map) {
        counts.push_back(pair.second);
    }

    // Sorts in descending order
    std::sort(counts.begin(), counts.end(), std::greater<>());

    if (counts[0] == 5) { // NOLINT
        strength = 6;     // NOLINT
    } else if (counts[0] == 4) {
        strength = 5; // NOLINT
    } else if (counts[0] == 3 && counts[1] == 2) {
        strength = 4;
    } else if (counts[0] == 3 && counts[1] == 1) {
        strength = 3;
    } else if (counts[0] == 2 && counts[1] == 2) {
        strength = 2;
    } else if (counts[0] == 2) {
        strength = 1;
    } else {
        strength = 0;
    }
};

auto compare_hands(const Hand &hand1, const Hand &hand2) -> bool {
    if (hand1.strength < hand2.strength) {
        return true;
    }

    if (hand1.strength > hand2.strength) {
        return false;
    }

    for (std::size_t i{0}; i < NUM_CARDS; i++) {
        const auto char1 = hand1.characters.at(i);
        const auto char2 = hand2.characters.at(i);

        if (card_strength.at(char1) > card_strength.at(char2)) {
            return false;
        }
        if (card_strength.at(char1) < card_strength.at(char2)) {
            return true;
        }
    }

    return false;
};

JHand::JHand(std::string_view hand) { // NOLINT(cppcoreguidelines-pro-type-member-init)
    std::unordered_map<char, int> counts_map;
    for (std::size_t i{0}; i < hand.size(); ++i) {
        characters.at(i) = hand[i];
        counts_map[hand[i]]++;
    }

    // Find number of J cards, add to the other most common one
    std::pair<char, int> most_common{'-', 0};
    for (auto &element : counts_map) {
        if (element.second > most_common.second && element.first != JOKER) {
            most_common = std::make_pair(element.first, element.second);
        }
    }
    if (most_common.first != '-' && counts_map.contains(JOKER)) {
        counts_map[most_common.first] += counts_map[JOKER];
        counts_map.erase(JOKER);
    }

    std::vector<int> counts;
    counts.reserve(counts_map.size());
    for (const auto &pair : counts_map) {
        counts.push_back(pair.second);
    }

    // Sorts in descending order
    std::sort(counts.begin(), counts.end(), std::greater<>());

    if (counts[0] == 5) { // NOLINT
        strength = 6;     // NOLINT
    } else if (counts[0] == 4) {
        strength = 5; // NOLINT
    } else if (counts[0] == 3 && counts[1] == 2) {
        strength = 4;
    } else if (counts[0] == 3 && counts[1] == 1) {
        strength = 3;
    } else if (counts[0] == 2 && counts[1] == 2) {
        strength = 2;
    } else if (counts[0] == 2) {
        strength = 1;
    } else {
        strength = 0;
    }
};

auto compare_jhands(const JHand &hand1, const JHand &hand2) -> bool {
    if (hand1.strength < hand2.strength) {
        return true;
    }

    if (hand1.strength > hand2.strength) {
        return false;
    }

    for (std::size_t i{0}; i < NUM_CARDS; i++) {
        const auto char1 = hand1.characters.at(i);
        const auto char2 = hand2.characters.at(i);

        if (jcard_strength.at(char1) > jcard_strength.at(char2)) {
            return false;
        }
        if (jcard_strength.at(char1) < jcard_strength.at(char2)) {
            return true;
        }
    }

    return false;
};