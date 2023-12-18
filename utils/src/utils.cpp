#include "utils/utils.h"

#include <chrono>
#include <filesystem> // for path
#include <fstream>    // IWYU pragma: keep
#include <iostream>
#include <string_view> // for basic_string_view
#include <vector>

namespace utils {
auto split_string(std::string_view str, char split_by) -> std::vector<std::string> {
    std::vector<std::string> parts;
    std::string::size_type prev_pos{0};
    std::string::size_type current_pos = str.find(split_by);

    while (current_pos != std::string::npos) {
        if (current_pos != prev_pos) {
            parts.push_back(
                static_cast<std::string>(str.substr(prev_pos, current_pos - prev_pos)));
        }
        prev_pos = current_pos + 1;
        current_pos = str.find(split_by, current_pos + 1);
    }

    if (str.length() != prev_pos) {
        parts.push_back(
            static_cast<std::string>(str.substr(prev_pos, current_pos - prev_pos)));
    }

    return parts;
}

auto read_lines_from_file(std::string_view fname) -> std::vector<std::string> {
    std::ifstream file{fname};
    std::vector<std::string> lines;

    // Leaving this like this until I learn about C++ exception/error handling :)))
    if (!file.is_open()) {
        std::cerr << "Can't read the file :("
                  << "\n";
        return lines;
    }

    std::string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }

    return lines;
}

auto Timer::reset() -> void { m_beg = Clock::now(); };

auto Timer::elapsed() const -> double {
    return std::chrono::duration_cast<Second>(Clock::now() - m_beg).count();
};
} // namespace utils
