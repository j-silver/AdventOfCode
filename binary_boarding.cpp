//
// Created by giuseppe on 05/12/2020.
//
#include "line.h"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <ranges>
#include <algorithm>
#include <iostream>
#include <set>

int id(std::string const& boardPass)
{
    int ID {0};
    for (int power {9}; auto&& c : boardPass) {
        if (c == 'B' || c == 'R') {
            ID += (1 << power);
        }
        --power;
    }
    return ID;
}

int main()
{
    const std::string example {"FBFBBFFRLR"};
    assert(example.length() == 10);
    int const exampleID {id(example)};
    assert(exampleID == 357);

    std::ifstream fileStream {std::filesystem::path {"../files/input5.txt"}};
    auto idsView {
              std::ranges::istream_view<Line>(fileStream)
            | std::ranges::views::transform(id)
    };
    std::set<int> seats;
    std::ranges::copy(idsView, std::inserter(seats, seats.begin()));
    std::cout << std::ranges::max(seats) << '\n';

    std::erase_if(seats, [](auto const& s){ return s >= (1 << 10) - 8 || s <= 7; });

    auto const found {
        std::ranges::adjacent_find(seats, [](auto const& s1, auto const& s2){
            return s2 - s1 == 2;
        })
    };

    std::cout << *(found) + 1 << '\n';
}