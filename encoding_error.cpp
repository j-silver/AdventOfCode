#include <filesystem>
#include <fstream>
#include <iostream>
#include <ranges>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>

constexpr int window {25};

bool is_sum(unsigned long long num, std::ranges::range auto const& slice)
{
    for (auto pos : std::ranges::iota_view{0UL, std::ranges::size(slice)-1}) {
        auto const tail {std::ranges::views::drop(slice, pos+1)};
        if (auto const found {std::ranges::find_if(tail,
                                                   [num,pos,&slice](auto const& second)
                                                   {
                                                        return slice[pos] + second == num;
                                                   })};
            found != std::ranges::end(tail))
        {
            return true;
        }
    }

    return false;
}

unsigned long long encryption_weakness(std::vector<unsigned long long> const& numbers,
                                       unsigned long long incorrect)
{
    unsigned long long smallest, largest {0};
    bool found {false};
    for (auto left : std::ranges::iota_view {0UL, std::ranges::size(numbers)-1}) {
        if (found) {
            break;
        }
        for (auto right : std::ranges::iota_view {left+1}) {
            auto const currentSum {std::accumulate(&numbers[left], &numbers[right], 0ULL)};
            if (currentSum > incorrect) {
                break;
            }
            if (currentSum == incorrect) {
                auto const extremes {std::minmax_element(&numbers[left], &numbers[right])};
                smallest = *extremes.first;
                largest = *extremes.second;
                found = true;
                break;
            }
        }
    }

    return smallest + largest;
}

int main(int argc, char* argv[])
{
    std::vector<unsigned long long> numbers;
    numbers.reserve(window);
    if (argc > 1) {
        std::fstream fileStream{std::filesystem::path{argv[1]}};
        std::ranges::copy(std::istream_iterator<unsigned long long>{fileStream}, std::istream_iterator<unsigned long long>{},
                          std::back_inserter(numbers));
    }
    else {
        std::ranges::copy(std::istream_iterator<unsigned long long>(std::cin), std::istream_iterator<unsigned long long>{},
                          std::back_inserter(numbers));
    }

    unsigned long long answer {0};

    for (auto left : std::ranges::iota_view{0UL, numbers.size() - window}) {
        auto const sliceView {
                std::ranges::views::drop(numbers, left) | std::ranges::views::take(window)
        };
        if (! is_sum(numbers[left + window], sliceView)) {
            answer = numbers[left + window];
            break;
        }
    }

    assert(answer == 15353384);
    std::cout << "Answer: " << answer << '\n';

    auto const encryptionWeakness {encryption_weakness(numbers, answer)};
    assert(encryptionWeakness == 2466556);
    std::cout << "Encryption weakness: " << encryptionWeakness << '\n';
}