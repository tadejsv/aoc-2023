#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <string_view>
#include <vector>

namespace utils {
auto split_string(std::string_view str, char split_by) -> std::vector<std::string>;
auto read_lines_from_file(std::string_view fname) -> std::vector<std::string>;
} // namespace utils
#endif // UTILS_H