//
// Created by giuseppe on 13/12/2020.
//

#include "depth_first_search.h"
#include "depth_first_search_fork.h"

template<typename Sentinel>
void depth_first_search(It const& it, Sentinel const& sentinel, ull& counter)
{
    if (std::next(it) == sentinel) {
        ++counter;
    }

    for (auto branch {std::next(it)};
         branch != sentinel && std::distance(it, branch) <= 3 && *branch - *it <= 3;
         ++branch)
    {
        depth_first_search(branch, sentinel, counter);
    }
}

