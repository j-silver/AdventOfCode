//
// Created by giuseppe on 20/12/2020.
//

#ifndef ADVENTOFCODE_DOCKING_H
#define ADVENTOFCODE_DOCKING_H

#include <cstdint>
#include <map>
#include <string>

struct BitMask {
    uint64_t bmask {0};
    uint64_t zeroes {0};
    uint64_t ones {0};
};

BitMask bitmask(std::string const& mask);

[[maybe_unused]] uint64_t write(std::string const& mask,
                                std::map<unsigned long /* addr */, uint64_t /* val */> const& data,
                                std::map<unsigned long /* addr */, uint64_t /* total */>& addresses);

#endif //ADVENTOFCODE_DOCKING_H
