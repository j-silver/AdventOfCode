//
// Created by giuseppe on 07/12/2020.
//

#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <regex>
#include <cassert>
#include "line.h"

namespace fs = std::filesystem;
namespace rg = std::ranges;

const std::regex areContained {R"(([0-9]+) (\w+ \w+) bag[s]?[.,])"};
const std::regex containRegex {R"((\w+ \w+) bags contain)"};
const std::regex notContaining {R"((\w+ \w+) bags contain no other bags.)"};


class Node {
public:
    explicit Node(std::string const& color)
        :   _hash {std::hash<std::string>{}(color)},
            _links{}
    {}

    template<typename ...L, typename N>
    std::enable_if_t<std::is_same_v<std::remove_cv_t<N>, std::shared_ptr<Node>>, void>
    add_nodes(N n, L...l)
    {
        _links.insert(n);
        add_nodes(l...);
    }

    template<typename N>
    std::enable_if_t<std::is_same_v<std::remove_cv_t<N>, std::shared_ptr<Node>>, void>
    add_nodes(N n)
    {
        _links.insert(n);
    }

    size_t hash() const
    {
        return _hash;
    }

    std::set<std::weak_ptr<Node>, std::owner_less<std::weak_ptr<Node>>> const& links() const
    {
        return _links;
    }

private:
    std::size_t _hash;
    std::set<std::weak_ptr<Node>, std::owner_less<std::weak_ptr<Node>>> _links;
};

class Graph {
public:
    using contained_t = std::set<std::pair<int, std::shared_ptr<Node>>>;

    explicit Graph(std::istream& is) : _nodes {}, _containing{}
    {
        auto view {rg::istream_view<Line>(is)};
        for (std::string const& line : view) {
            if (std::smatch matches; !line.empty() && std::regex_search(line, matches, containRegex)) {
                auto const container {matches[1].str()};
                std::shared_ptr<Node> containerNode;
                if (auto found {std::find_if(_nodes.cbegin(), _nodes.cend(),
                                            [&container](auto const& n)
                                            {
                                                return n->hash() == std::hash<std::string>{}(container);
                                            })};
                    found == _nodes.cend())
                {
                    containerNode = std::make_shared<Node>(container);
                    _nodes.insert(containerNode);
                }
                else {
                    containerNode = *found;
                }
                for (std::sregex_iterator it {line.cbegin(), line.cend(), areContained};
                     it != std::sregex_iterator{};
                     ++it)
                {
                    auto const contained{(*it)[2].str()};
                    auto const number {std::stoi((*it)[1].str())};
                    if (auto containedFound {std::find_if(_nodes.cbegin(), _nodes.cend(),
                                                [&contained](auto const& r)
                                                {
                                                    return r->hash() == std::hash<std::string>{}(contained);
                                                })};
                        containedFound == _nodes.cend())
                    {
                        auto const containedNode {std::make_shared<Node>(contained)};
                        containedNode->add_nodes(containerNode);
                        _nodes.insert(containedNode);
                        _containing[containerNode].insert(std::make_pair(number, containedNode));
                    }
                    else {
                        (*containedFound)->add_nodes(containerNode);
                        _containing[containerNode].insert(std::make_pair(number, *containedFound));
                    }
                }
            }
            if (std::smatch matches; std::regex_search(line, matches, notContaining)) {
                auto const notContainer {matches[1].str()};
                if (auto found {std::find_if(_nodes.cbegin(), _nodes.cend(),
                                [&notContainer](auto const& n){ return n->hash() == std::hash<std::string>{}(notContainer); })};
                    found == _nodes.cend())
                {
                    _nodes.insert(std::make_shared<Node>(notContainer));
                }
            }
        }
    }

    [[nodiscard]] std::set<std::shared_ptr<Node>> const& nodes() const
    {
        return _nodes;
    }

    [[nodiscard]] std::map<std::shared_ptr<Node>, contained_t> const& containing() const
    {
        return _containing;
    }

private:

    std::set<std::shared_ptr<Node>> _nodes;
    std::map<std::shared_ptr<Node>, contained_t> _containing;
};

template<typename VisitorType>
auto breadth_first_search(std::shared_ptr<Node> const& root,
                          VisitorType& visitFunction)
{
    std::deque<std::shared_ptr<Node>> queue;
    using result_type = decltype(std::declval<VisitorType>()(root));
    std::map<std::shared_ptr<Node>, result_type> visited;

    visited[root] = visitFunction(root);
    queue.push_back(root);

    while (! queue.empty()) {
        auto const currentRoot {queue.front()};
        queue.pop_front();
        for (auto const& link : currentRoot->links()) {
            auto observe {link.lock()};
            visited[observe] = visitFunction(observe);
            queue.emplace_back(link);
        }
    }

    return visited;
}

struct ContainedVisitor {
    ContainedVisitor() = default;

    bool operator()(std::shared_ptr<Node> const& n [[maybe_unused]]) const
    {
        return true;
    }
};

int count_contained(std::map<std::shared_ptr<Node>, Graph::contained_t> const& containing,
                    Graph::contained_t const& set)
{
    int num {0};
    for (auto& containedBags : set) {
        num += containedBags.first;
        if (auto const found {containing.find(containedBags.second)}; found != containing.end()) {
            num += containedBags.first * count_contained(containing, found->second);
        }
    }

    return num;
}

int main(int argc, char* argv[])
{
    auto const graph {
            [argc, argv]() {
                if (argc > 1) {
                    if (fs::path const path{argv[1]}; fs::exists(path)) {
                        std::ifstream fileStream{path};
                        return Graph(fileStream);
                    }
                }
                return Graph(std::cin);
            }()
    };

    auto const shinyGoldHash {std::hash<std::string>{}("shiny gold")};
    auto const node {std::find_if(graph.nodes().cbegin(), graph.nodes().cend(),
                                     [shinyGoldHash](auto const& n){ return n->hash() == shinyGoldHash; })};

    if (node == graph.nodes().cend()) {
        std::terminate();
    }

    ContainedVisitor visitor;
    auto const visited {breadth_first_search(*node, visitor)};
    auto const containedBy {visited.size()-1};
    assert(containedBy == 296);
    std::cout << "Shiny yellow bag can be contained by " << containedBy << " bags\n";

    std::cout << "Number of containers: " << graph.containing().size() << '\n';

    auto const& containers {graph.containing()};
    auto const firstContainer {containers.find(*node)};
    int bags {count_contained(containers, firstContainer->second)};

    assert(bags == 9339);
    std::cout << "Shiny yellow bag must contain " << bags << " bags\n";
}

