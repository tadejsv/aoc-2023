#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "utils/utils.h"

struct GameMax {
    int red;
    int blue;
    int green;
};

auto
get_game_num(const std::string& game_num_part) -> int {
    return std::stoi(utils::split_string(game_num_part, ' ').at(1));
}

auto
get_game_max(const std::vector<std::string>& iters) -> GameMax {
    auto game_max = GameMax{ 0, 0, 0 };

    for (const auto& iter : iters) {
        for (const auto& dice : utils::split_string(iter, ',')) {
            auto num_color = utils::split_string(dice, ' ');
            auto num = std::stoi(num_color.at(0));
            auto color = num_color.at(1);

            if (color == "green" && num > game_max.green) {
                game_max.green = num;
            } else if (color == "red" && num > game_max.red) {
                game_max.red = num;
            } else if (color == "blue" && num > game_max.blue) {
                game_max.blue = num;
            }
        }
    }

    return game_max;
}

int
main() {  // NOLINT
    const std::string fname{ "../../day2/input.txt" };

    constexpr int red{ 12 };
    constexpr int blue{ 14 };
    constexpr int green{ 13 };

    std::unordered_map<int, GameMax> games;

    for (const auto& line : utils::read_lines_from_file(fname)) {
        const auto name_iters_split = utils::split_string(line, ':');
        const auto game_num = get_game_num(name_iters_split.at(0));

        const auto iters = utils::split_string(name_iters_split.at(1), ';');
        games[game_num] = get_game_max(iters);
    }

    int sum_possible{ 0 };
    for (const auto& num_game_pair : games) {
        auto game = num_game_pair.second;
        if (game.red <= red && game.blue <= blue && game.green <= green) {
            sum_possible += num_game_pair.first;
        }
    }

    std::cout << sum_possible << "\n";
    return 0;
}