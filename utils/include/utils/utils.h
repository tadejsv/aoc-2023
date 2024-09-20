#ifndef UTILS_H
#define UTILS_H

#include <chrono>
#include <ratio>
#include <string>
#include <string_view>
#include <vector>

namespace utils {
    auto split_string(std::string_view str, char split_by) -> std::vector<std::string>;
    auto read_lines_from_file(std::string_view fname) -> std::vector<std::string>;

    class Timer {
    private:

        using Clock = std::chrono::steady_clock;
        using Second = std::chrono::duration<double, std::ratio<1>>;

        std::chrono::time_point<Clock> m_beg{ Clock::now() };

    public:

        auto reset() -> void;
        [[nodiscard]] auto elapsed() const -> double;
    };

}  // namespace utils
#endif  // UTILS_H