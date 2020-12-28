//
// Created by giuseppe on 21/12/2020.
//
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <numeric>
#include <cassert>
#include "docking.h"

std::regex const memPattern {R"(mem\[([0-9]+)\] = ([0-9]+))"};

std::map<unsigned long, uint64_t> write_to_memory(std::istream& inputStream)
{
    std::map<unsigned long, uint64_t> addresses;
    assert(addresses.max_size() > 585);

    std::string currentMask;
    std::map<unsigned long, uint64_t> mem;

    for (std::string line; inputStream.good(); ) {
        std::getline(inputStream, line);
        if (line.empty()) {
            continue;
        }
        else if (line.starts_with("mask")) {
            if (! currentMask.empty()) {
                write(currentMask, mem, addresses); // write previous data
                mem.clear(); // clear the buffer
            }
            // update mask
            currentMask = line.substr(7);
        }
        else if (line.starts_with("mem")) {
            std::smatch matches;
            std::regex_match(line, matches, memPattern);
            mem[std::stoul(matches[1].str())] = std::stoull(matches[2].str());
        }
    }

    // Last write
    write(currentMask, mem, addresses);

    return addresses;
}

int main()
{

    std::ifstream fileStream {std::filesystem::path {"../../files/input14.txt"}};
    auto const addresses {write_to_memory(fileStream)};

    auto const result {
            std::accumulate(addresses.cbegin(), addresses.cend(), uint64_t {0},
                            [](auto init, auto const& addr) { return init + addr.second; })
    };

    std::cout << result << '\n';
}
