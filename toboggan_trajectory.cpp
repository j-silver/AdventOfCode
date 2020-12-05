//
// Created by giuseppe on 04/12/2020.
//
#include <fstream>
#include <filesystem>
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include "line.h"

using field = std::vector<std::vector<bool>>;
using sz_t = field::size_type;
using slope = std::pair<sz_t, sz_t>;

const std::vector<slope> slopes {
    {1, 1}, {3, 1}, {5, 1}, {7, 1}, {1, 2}
};


unsigned long number_of_trees(slope const& s, field const& f)
{
    unsigned long trees {0};
    auto const [right, down] {s};
    auto const width {f.front().size()};
    auto row {f.begin()};
    for (auto pos {right};
         std::distance(row, f.end()) > static_cast<long>(down);
         pos = pos + right < width ? pos + right : (pos + right)%width)
    {
        std::advance(row, down);
        if (row->at(pos)) {
            ++trees;
        }
    }

    return trees;
}

int main()
{
    std::ifstream fileStream {std::filesystem::path {"../files/input3.txt"}};
    std::vector<std::vector<bool>> pattern;
    std::vector<bool> row;
    for (std::string line : std::ranges::istream_view<Line>(fileStream)) {
        std::ranges::transform(line, std::inserter(row, row.begin()),
                            [](auto const& c){ return c == '#'; });
        pattern.push_back(row);
    }

    auto const product {
        std::accumulate(slopes.cbegin(), slopes.cend(), 1UL,
                        [&pattern](auto init, auto const& s){ return init*number_of_trees(s, pattern); })
    };

    std::cout << product << '\n';
}