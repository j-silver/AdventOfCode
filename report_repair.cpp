#include <iostream>
#include <fstream>
#include <filesystem>
#include <list>
#include <algorithm>

using namespace std::ranges;

constexpr int year {2020};

int main()
{
    std::ifstream fileStream {std::filesystem::path {"../files/input1.txt"}};
    std::istream_iterator<int> it {fileStream}, endIt {};
    std::list<int> entries;
    std::copy_if(it, endIt, std::back_inserter(entries), [](auto const& input){ return input < year; });
    for (auto e {entries.begin()}; e != entries.end(); ++e) {
        if (auto const found {find_if(subrange(e, entries.end()), [a=*e](auto const& n){ return a + n == year; })};
            found != entries.end())
        {
            std::cout << *e * *found << '\n';
            break;
        }
    }
    for (auto e {entries.begin()}; e != entries.end(); ++e) {
        for (auto b {std::next(e)}; b != entries.end(); ++b) {
            if (*e + *b < year) {
                if (auto const third {find_if(subrange(std::next(b), entries.end()),
                                     [A = *e, B = *b](auto const& C) { return A + B + C == year; })};
                    third != entries.end())
                {
                    std::cout << *e * *b * *third << '\n';
                    break;
                }
            }
        }
    }
}
