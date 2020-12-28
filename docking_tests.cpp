//
// Created by giuseppe on 20/12/2020.
//

#include <catch2/catch.hpp>
#include "docking.h"

TEST_CASE("Wrong mask's length")
{
    std::string const mask {"XXX001XX"};
    std::map<unsigned long, uint64_t> const mem;
    std::map<unsigned long, uint64_t> addresess;

    CHECK_THROWS_AS(write(mask, mem, addresess), std::invalid_argument);
}

TEST_CASE("Wrong mask")
{
    std::string const mask {"XXY0112XX"};
    CHECK_THROWS_AS(bitmask(mask), std::invalid_argument);
}

TEST_CASE("First test")
{
    std::vector<std::pair<unsigned long /* addr */, uint64_t /* val */>> const mem {{8,11}, {7, 101}, {8, 0}};
    std::string const mask {"XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X"};

    auto const bmask {bitmask(mask)};
    REQUIRE(bmask.zeroes == 2);
    REQUIRE(bmask.ones == 64);
    REQUIRE(bmask.bmask % 2 == 1);

    std::map<unsigned long, uint64_t> data;
    for (auto const& datum : mem) {
        data.insert_or_assign(datum.first, datum.second);
    }

    std::map<unsigned long, uint64_t> addresses;
    auto const result {write(mask, data, addresses)};
    CHECK(result == 165);
    CHECK(addresses[7] == 101);
    CHECK(addresses[8] == 64);
}