#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "utils/utils.h"

struct Point {
    long x;
    long y;
};

// The first and last point of vertices should be the same (closed loop)
auto
fill_area(const std::vector<Point>& vertices) -> long {
    long double_area{ 0 };
    long boundary{ 0 };
    for (std::size_t i{ 0 }; i < vertices.size() - 1; ++i) {
        double_area += (vertices[i].x - vertices[i + 1].x) * (vertices[i].y + vertices[i + 1].y);
        boundary += abs(vertices[i].x - vertices[i + 1].x) + abs(vertices[i].y - vertices[i + 1].y);
    }

    long double_interior_points = abs(double_area) + 2 - boundary;
    long fill_area = double_interior_points / 2 + boundary;

    return fill_area;
}

int
main()  // NOLINT
{
    const auto lines = utils::read_lines_from_file("input.txt");

    Point current{ 0, 0 };
    std::vector<Point> points{ current };

    const auto start{ std::chrono::high_resolution_clock::now() };
    for (const auto& line : lines) {
        const auto line_parts = utils::split_string(line, ' ');

        const auto steps = std::stol(line_parts[2].substr(2, 5), nullptr, 16);
        const auto direction = line_parts[2][7] - '0';

        if (direction == 0) {
            current.x += steps;
        } else if (direction == 2) {
            current.x -= steps;
        } else if (direction == 1) {
            current.y += steps;
        } else if (direction == 3) {
            current.y -= steps;
        }

        points.emplace_back(current);
    }

    long area = fill_area(points);
    const auto end{ std::chrono::high_resolution_clock::now() };
    auto ms_int = duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "fill: " << area << "\n";
    std::cout << "Time taken: " << ms_int << " \n";

    return 0;
};
