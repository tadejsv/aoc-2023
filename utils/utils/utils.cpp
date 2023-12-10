#include "utils.h"

#include <fstream> // IWYU pragma: keep
#include <iostream>
#include <vector>

namespace utils {
auto split_string(const std::string &str, char split_by) -> std::vector<std::string> {
    std::vector<std::string> parts;
    std::string::size_type prev_pos{0};
    std::string::size_type current_pos = str.find(split_by);

    while (current_pos != std::string::npos) {
        if (current_pos != prev_pos) {
            parts.push_back(str.substr(prev_pos, current_pos - prev_pos));
        }
        prev_pos = current_pos + 1;
        current_pos = str.find(split_by, current_pos + 1);
    }

    if (str.length() != prev_pos) {
        parts.push_back(str.substr(prev_pos, current_pos - prev_pos));
    }

    return parts;
}

auto read_lines_from_file(const std::string &fname) -> std::vector<std::string> {
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
} // namespace utils
