//
// Created by giuseppe on 13/12/2020.
//
#include "catch2/catch.hpp"
#include "line.h"
#include "depth_first_search.cpp"
#include "bifurcations.h"
#include "groups.h"
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>

TEST_CASE("Simple case: 1, 2") {
    std::istringstream is {"1 2"};
    auto input {std::ranges::istream_view<int>(is)};
    std::vector<int> adapters {0};
    std::ranges::copy(input, std::back_inserter(adapters));
    std::ranges::sort(adapters);
    adapters.push_back(adapters.back()+3);
    std::vector<int> differences;
    std::adjacent_difference(adapters.cbegin(), adapters.cend(), std::back_inserter(differences));
    auto const oneJolt {std::ranges::count(differences, 1)};
    REQUIRE(oneJolt == 2);
    auto const threeJolts {std::ranges::count(differences, 3)};
    REQUIRE(threeJolts == 1);
    REQUIRE(oneJolt*threeJolts == 2);

    ull arrangements {0};
    depth_first_search(adapters.cbegin(), adapters.cend(), arrangements);
    REQUIRE(arrangements == 2);
}

TEST_CASE("Simple case: 1, 2, 3") {
    std::istringstream is {"1 2 3"};
    auto input {std::ranges::istream_view<int>(is)};
    std::vector<int> adapters {0};
    std::ranges::copy(input, std::back_inserter(adapters));
    std::ranges::sort(adapters);
    adapters.push_back(adapters.back()+3);
    std::vector<int> differences;
    std::adjacent_difference(adapters.cbegin(), adapters.cend(), std::back_inserter(differences));
    auto const oneJolt {std::ranges::count(differences, 1)};
    REQUIRE(oneJolt == 3);
    auto const threeJolts {std::ranges::count(differences, 3)};
    REQUIRE(threeJolts == 1);
    REQUIRE(oneJolt*threeJolts == 3);

    ull arrangements {0};
    depth_first_search(adapters.cbegin(), adapters.cend(), arrangements);
    REQUIRE(arrangements == 4);
}

TEST_CASE("Test with file test10.txt") {
    std::ifstream fileStream {"../../files/test10.txt"};
    REQUIRE(fileStream.good());
    auto input {std::ranges::istream_view<Line>(fileStream)};
    std::vector<int> adapters {0};
    std::ranges::transform(input, std::back_inserter(adapters), [](auto const& line){ return std::stoi(line); });
    std::ranges::sort(adapters);
    adapters.push_back(adapters.back()+3);
    std::vector<int> differences;
    std::adjacent_difference(adapters.cbegin(), adapters.cend(), std::back_inserter(differences));
    auto const oneJolt {std::ranges::count(differences, 1)};
    REQUIRE(oneJolt == 7);
    auto const threeJolts {std::ranges::count(differences, 3)};
    REQUIRE(threeJolts == 5);
    REQUIRE(oneJolt*threeJolts == 35);

    ull arrangements {0};
    depth_first_search(adapters.cbegin(), adapters.cend(), arrangements);
    REQUIRE(arrangements == 8);
}

TEST_CASE("Test with file longer_test_for_day10.txt")
{
    std::ifstream fileStream {"../../files/longer_test_for_day10.txt"};
    REQUIRE(fileStream.good());
    auto input {std::ranges::istream_view<Line>(fileStream)};
    std::vector<int> adapters {0};
    std::ranges::transform(input, std::back_inserter(adapters), [](auto const& line){ return std::stoi(line); });
    std::ranges::sort(adapters);
    adapters.push_back(adapters.back()+3);
    std::vector<int> differences;
    std::adjacent_difference(adapters.cbegin(), adapters.cend(), std::back_inserter(differences));
    auto const oneJolt {std::ranges::count(differences, 1)};
    REQUIRE(oneJolt == 22);
    auto const threeJolts {std::ranges::count(differences, 3)};
    REQUIRE(threeJolts == 10);

    ull arrangements {0};
    depth_first_search(adapters.cbegin(), adapters.cend(), arrangements);
    REQUIRE(arrangements == 19208);
}

TEST_CASE("Simple test {1, 2} with 'bifurcation' method")
{
    std::istringstream is {"1 2"};
    auto input {std::ranges::istream_view<int>(is)};
    std::vector<int> adapters {0};
    std::ranges::copy(input, std::back_inserter(adapters));
    std::ranges::sort(adapters);
    adapters.push_back(adapters.back()+3);

    auto const paths {bifurcations_count(adapters)};
    REQUIRE(paths == 2);
}

TEST_CASE("Simple test {1, 2, 3} with 'bifurcation' method")
{
    std::istringstream is {"1 2 3"};
    auto input {std::ranges::istream_view<int>(is)};
    std::vector<int> adapters {0};
    std::ranges::copy(input, std::back_inserter(adapters));
    std::ranges::sort(adapters);
    adapters.push_back(adapters.back()+3);

    auto const paths {bifurcations_count(adapters)};
    REQUIRE(paths == 4);
}

TEST_CASE("Test with file test10.txt with 'bifurcation' method")
{
    std::ifstream fileStream {"../../files/test10.txt"};
    REQUIRE(fileStream.good());
    auto input {std::ranges::istream_view<Line>(fileStream)};
    std::vector<int> adapters {0};
    std::ranges::transform(input, std::back_inserter(adapters), [](auto const& line){ return std::stoi(line); });
    std::ranges::sort(adapters);
    adapters.push_back(adapters.back()+3);

    auto const paths {bifurcations_count(adapters)};
    REQUIRE(paths == 8);
}

TEST_CASE("Test with file longer_test_for_day10.txt with 'bifurcation' method")
{
    std::ifstream fileStream {"../../files/longer_test_for_day10.txt"};
    REQUIRE(fileStream.good());
    auto input{std::ranges::istream_view<Line>(fileStream)};
    std::vector<int> adapters{0};
    std::ranges::transform(input, std::back_inserter(adapters), [](auto const& line) { return std::stoi(line); });
    std::ranges::sort(adapters);
    adapters.push_back(adapters.back() + 3);

    auto const paths {bifurcations_count(adapters)};
    REQUIRE(paths == 19208);
}

TEST_CASE("Simple test {1, 2} with 'groups' method")
{
    std::istringstream is {"1 2"};
    auto input {std::ranges::istream_view<int>(is)};
    std::vector<int> adapters {0};
    std::ranges::copy(input, std::back_inserter(adapters));
    std::ranges::sort(adapters);
    adapters.push_back(adapters.back() + 3);

    auto const answer2 {count_arrangements(adapters)};
    CHECK(answer2 == 2);
}


TEST_CASE("Simple test {1, 2, 3} with 'groups' method")
{
    std::istringstream is {"1 2 3"};
    auto input {std::ranges::istream_view<int>(is)};
    std::vector<int> adapters {0};
    std::ranges::copy(input, std::back_inserter(adapters));
    std::ranges::sort(adapters);
    adapters.push_back(adapters.back() + 3);

    auto const answer2 {count_arrangements(adapters)};
    CHECK(answer2 == 4);
}

TEST_CASE("2 4 6")
{
    std::istringstream is {"2 4 6"};
    auto input {std::ranges::istream_view<int>(is)};
    std::vector<int> adapters {0};
    std::ranges::copy(input, std::back_inserter(adapters));
    std::ranges::sort(adapters);
    adapters.push_back(adapters.back() + 3);

    auto const answer2 {count_arrangements(adapters)};
    CHECK(answer2 == 1);
}

TEST_CASE("Test with file test10.txt with 'groups' method")
{
    std::ifstream fileStream {"../../files/test10.txt"};
    REQUIRE(fileStream.good());
    auto input {std::ranges::istream_view<Line>(fileStream)};
    std::vector<int> adapters {0};
    std::ranges::transform(input, std::back_inserter(adapters), [](auto const& line){ return std::stoi(line); });
    std::ranges::sort(adapters);
    adapters.push_back(adapters.back()+3);

    CHECK(count_arrangements(adapters) == 8);
}