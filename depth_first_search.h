//
// Created by giuseppe on 13/12/2020.
//

#ifndef ADVENTOFCODE_DEPTH_FIRST_SEARCH_H
#define ADVENTOFCODE_DEPTH_FIRST_SEARCH_H

#include <vector>

using It = std::vector<int>::const_iterator;
using ull = unsigned long long;

template<typename Sentinel>
void depth_first_search(It const& it, Sentinel const& sentinel, ull& counter);


#endif //ADVENTOFCODE_DEPTH_FIRST_SEARCH_H
