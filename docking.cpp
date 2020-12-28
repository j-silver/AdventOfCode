//
// Created by giuseppe on 20/12/2020.
//
#include <stdexcept>
#include <map>
#include <algorithm>
#include <numeric>
#include <iostream>
#include "docking.h"

using namespace std::string_literals;

BitMask bitmask(std::string const& mask)
{
    BitMask bm;

    for (size_t exponent {0}; exponent < mask.size(); ++exponent) {
        auto const index {mask.size() - 1 - exponent};
        if (mask[index] == 'X') {
            bm.bmask |= (1 << exponent);
        }
        else if (mask[index] == '1') {
            bm.ones |= (1 << exponent);
        }
        else if (mask[index] == '0') {
            bm.zeroes |= (1 << exponent);
        }
        else {
            throw std::invalid_argument {"Bad mask: value "s + mask[index] + " at index "s + std::to_string(index)};
        }
    }

    return bm;
}

uint64_t write(std::string const& mask,
               std::map<unsigned long, uint64_t> const& data,
               std::map<unsigned long /* addr */, uint64_t /* total */>& addresses)
{
    if (mask.length() != 36) {
        throw std::invalid_argument {"Mask must be 36-bit"};
    }

    if (data.empty()) {
        return 0;
    }

    auto const bm {bitmask(mask)};

    for (auto const& datum : data) {
        addresses[datum.first] = 0ULL;
        addresses[datum.first] |= (datum.second & bm.bmask);
        addresses[datum.first] |= bm.ones;
        addresses[datum.first] |= bm.zeroes;
        addresses[datum.first] -= bm.zeroes;
    }

    return std::accumulate(addresses.cbegin(), addresses.cend(), 0UL,
                       [](auto init, auto const& add){ return init + add.second; });
}