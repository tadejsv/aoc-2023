#include "utils/utils.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

struct RangeIndex {
    long long key_start;
    long long val_start;
    long long length;
};

class RangeMap {
  private:
    std::vector<RangeIndex> ranges;

  public:
    auto add_range(const RangeIndex &range_index) -> void {
        ranges.push_back(range_index);
    };
    auto find(const long long key) -> long long {
        for (auto const &index : ranges) {
            if (key >= index.key_start && key < index.key_start + index.length) {
                return index.val_start + (key - index.key_start);
            }
        }

        return key;
    };
};

int main() { // NOLINT
    const auto lines = utils::read_lines_from_file("input.txt");

    std::vector<long long> seeds;
    RangeMap seed2soil;
    RangeMap soil2fertilizer;
    RangeMap fertilizer2water;
    RangeMap water2light;
    RangeMap light2temperature;
    RangeMap temperature2humidty;
    RangeMap humidty2location;

    RangeMap *current_range_map{nullptr};

    for (size_t i{0}; i < lines.size(); ++i) {
        const auto &line{lines.at(i)};
        if (line.contains("seeds:")) {
            const auto name_seeds = utils::split_string(line, ':');
            const auto seeds_str = utils::split_string(name_seeds.at(1), ' ');

            for (const auto &seed_str : seeds_str) {
                seeds.push_back(std::stoll(seed_str));
            }
        } else if (line.size() < 3) {
            continue;
        } else if (line.contains("seed-to-soil map:")) {
            current_range_map = &seed2soil;
        } else if (line.contains("soil-to-fertilizer map:")) {
            current_range_map = &soil2fertilizer;
        } else if (line.contains("fertilizer-to-water map:")) {
            current_range_map = &fertilizer2water;
        } else if (line.contains("water-to-light map:")) {
            current_range_map = &water2light;
        } else if (line.contains("light-to-temperature map:")) {
            current_range_map = &light2temperature;
        } else if (line.contains("temperature-to-humidity map:")) {
            current_range_map = &temperature2humidty;
        } else if (line.contains("humidity-to-location map:")) {
            current_range_map = &humidty2location;
        } else { // Assuming we have a row of 3 numbers
            auto const numbers_str = utils::split_string(line, ' ');
            if (current_range_map != nullptr) {
                current_range_map->add_range(
                    RangeIndex{.key_start = std::stoll(numbers_str.at(1)),
                               .val_start = std::stoll(numbers_str.at(0)),
                               .length = std::stoll(numbers_str.at(2))});
            } else {
                std::cout << "oh no";
            }
        }
    }

    utils::Timer timer;

    std::vector<long long> locations;
    for (auto seed : seeds) {
        auto const soil = seed2soil.find(seed);
        auto const fertilizer = soil2fertilizer.find(soil);
        auto const water = fertilizer2water.find(fertilizer);
        auto const light = water2light.find(water);
        auto const temperature = light2temperature.find(light);
        auto const humidity = temperature2humidty.find(temperature);
        auto const location = humidty2location.find(humidity);

        locations.push_back(location);
    }
    auto min_el = *std::ranges::min_element(locations);

    constexpr double mus_in_s{1000000};
    double time_elapsed{timer.elapsed() * mus_in_s / static_cast<double>(seeds.size())};
    std::cout << "Time elapsed: " << time_elapsed << " μs\n";
    std::cout << min_el << " ";

    return 0;
}