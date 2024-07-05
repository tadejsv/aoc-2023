#include "utils/utils.h"
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

struct Point {
    int x;
    int y;
};

// The first and last point of vertices should be the same (closed loop)
auto polygon_area(const std::vector<Point> &vertices) -> double {
    int double_area{0};
    for (std::size_t i{0}; i < vertices.size() - 1; ++i) {
        double_area +=
            (vertices[i].x - vertices[i + 1].x) * (vertices[i].y + vertices[i + 1].y);
    }

    return 0.5 * static_cast<double>(abs(double_area)); // NOLINT
}

int main() // NOLINT
{
    const auto lines = utils::read_lines_from_file("input.txt");

    Point current{0, 0};
    std::vector<Point> points{current};
    int boundary{0};

    for (const auto &line : lines) {
        const auto line_parts = utils::split_string(line, ' ');
        const auto &direction{line_parts[0]};
        const int steps{std::stoi(line_parts[1])};
        boundary += steps;

        if (direction == "R") {
            current.x += steps;
        } else if (direction == "L") {
            current.x -= steps;
        } else if (direction == "D") {
            current.y += steps;
        } else if (direction == "U") {
            current.y -= steps;
        }

        points.emplace_back(current);
    }

    double area = polygon_area(points);
    double interior_points = area + 1 - boundary * 0.5; // NOLINT
    int fill_area = static_cast<int>(interior_points + boundary);

    std::cout << "Area: " << area << " boundary: " << boundary << "\n";
    std::cout << "interior: " << interior_points << " fill: " << fill_area << "\n";

    return 0;
};
