#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

namespace utils {
auto split_string(const std::string &str, char split_by) -> std::vector<std::string>;
auto read_lines_from_file(const std::string &fname) -> std::vector<std::string>;
} // namespace utils
#endif // UTILS_H