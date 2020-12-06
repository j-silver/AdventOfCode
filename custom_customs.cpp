//
// Created by giuseppe on 06/12/2020.
//
#include <filesystem>
#include <set>
#include <list>
#include <fstream>
#include <iostream>
#include <numeric>

namespace fs = std::filesystem;
using Person = std::string;
using Group = std::multiset<Person>;

template<typename Streamable>
requires std::derived_from<Streamable, std::istream>
std::list<Group> groups(Streamable& is)
{
    std::list<Group> gs;
    for (Group grp; is.good(); grp.clear()) {
        for (Person person; is.good(); ) {
            std::getline(is, person);
            if (person.empty()) {
                break;
            }
            grp.insert(person);
        }
        gs.push_back(grp);
    }

    return gs;
}

int main(int argc, char* argv[])
{
    std::list<Group> grps;
    if (argc > 1) {
        if (fs::path const path {argv[1]}; fs::exists(path)) {
            std::ifstream fileStream {path};
            grps = groups(fileStream);
        }
    }
    else {
        grps = groups(std::cin);
    }

    unsigned long answer1 {0};
    for (auto const& group : grps) {
        std::set<char> allUnique;
        for (auto const& p : group) {
            allUnique.insert(p.cbegin(), p.cend());
        }
        answer1 += allUnique.size();
    }

    long answer2 {0};
    for (auto const& group : grps) {
        auto const firstPerson {group.cbegin()};
            answer2 += std::accumulate(
                        firstPerson->cbegin(), firstPerson->cend(), 0,
                    [firstPerson, &group](auto init, auto const& c)
                        {
                            if (std::all_of(std::next(firstPerson), group.cend(),
                                            [c](auto const& p)
                                            { return p.find(c) != std::string::npos; }))
                                return ++init;
                            return init;
                        });
    }

    std::cout << answer1 << '\n' << answer2 << '\n';
}
