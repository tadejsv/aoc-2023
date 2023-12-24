#ifndef DAY7_H
#define DAY7_H

#include <array>
#include <cstddef>
#include <string_view>
#include <unordered_map>

constexpr std::size_t NUM_CARDS{5};

class Hand {
  public:
    int strength;
    std::array<char, NUM_CARDS> characters;

    Hand(std::string_view hand);
};

class JHand {
  public:
    int strength;
    std::array<char, NUM_CARDS> characters;

    JHand(std::string_view hand);
};

// False if first hands is stronger, true if second one is, false on ties
auto compare_hands(const Hand &hand1, const Hand &hand2) -> bool;
auto compare_jhands(const JHand &hand1, const JHand &hand2) -> bool;

#endif // DAY7_H