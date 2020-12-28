#include <algorithm>
#include <iostream>
#include <numeric>
#include <cassert>
#include <deque>
#include "depth_first_search.cpp"

using namespace std::ranges;
using It = std::vector<int>::const_iterator;
using ull = unsigned long long;

It last_fork(std::vector<int> const& adapters)
{
    assert(adapters.size() >= 4);
    for (auto pos {std::next(adapters.rbegin(), 3)}; pos != adapters.rend(); ++pos) {
        if (*std::prev(pos, 2) - *pos <= 3)
            return pos.base();
    }

    return adapters.begin();
}

int main()
{
    auto input {istream_view<int>(std::cin)};
    std::vector<int> adapters {0};
    copy(input, std::back_inserter(adapters));
    sort(adapters);
    adapters.push_back(adapters.back()+3);
    std::vector<int> differences;
    std::adjacent_difference(adapters.cbegin(), adapters.cend(), std::back_inserter(differences));
    auto const oneJoltDifferences {count(differences, 1)};
    auto const threeJoltDifferences {count(differences, 3)};
    auto const answer {oneJoltDifferences*threeJoltDifferences};
//    assert(answer == 1920);
    std::cout << "First answer: " << answer << std::endl;
    ull arrangements {0};
//    depth_first_search(adapters.cbegin(), adapters.cend(), arrangements);
    depth_first_search(adapters.cbegin(), last_fork(adapters), arrangements);
    std::cout << "Arrangements: " << arrangements << '\n';
}
