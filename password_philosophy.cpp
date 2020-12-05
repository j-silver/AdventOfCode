//
// Created by giuseppe on 03/12/2020.
//
#include <fstream>
#include <iostream>
#include <filesystem>
#include <regex>
#include "line.h"

std::regex const pattern {R"(([0-9]+)-([0-9]+) (.): (\w+))"};

bool check_password(std::string const& password)
{
    if (std::smatch m; !std::regex_search(password, m, pattern)) {
        return false;
    }
    else {
        auto const [min, max, letter, pass] {std::make_tuple(m[1].str(), m[2].str(), m[3].str(), m[4].str())};
        int count {0};
        auto const increment {
                [max, letter, &count](auto const& c) mutable
                   {
                       if (letter[0] == c) {
                           ++count;
                       }
                       return count > std::stoi(max);
                   }
        };
        if (auto const finished [[maybe_unused]] {std::ranges::any_of(pass, increment)};
            count < std::stoi(min) || count > std::stoi(max))
        {
            return false;
        }
        return true;
    }
}

bool check_password_again(std::string const& password)
{
    if (std::smatch m; !std::regex_search(password, m, pattern)) {
        return false;
    }
    else {
        auto const [first, second, letter, pass] {std::make_tuple(m[1].str(), m[2].str(), m[3].str(), m[4].str())};
        auto const i {static_cast<std::string::size_type>(std::stoi(first))};
        auto const j {static_cast<std::string::size_type>(std::stoi(second))};
        auto const l {letter[0]};
        if (pass.length() < j) {
            return pass[i-1] == l;
        }
        return (pass[i-1] == l && pass[j-1] != l) || (pass[i-1] != l && pass[j-1] == l);
    }
}

int main()
{
    std::ifstream fileStream {std::filesystem::path {"../files/input2.txt"}};
    int count1 {0};
    int count2 {0};
    auto lines {std::ranges::istream_view<Line>(fileStream)};
    std::ranges::for_each(lines, [&count1, &count2](auto const& l)
    {
        if (check_password(l)) ++count1;
        if (check_password_again(l)) ++count2;
    });

    std::cout << count1 << ' ' << count2 <<'\n';
}