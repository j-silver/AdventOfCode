//
// Created by giuseppe on 09/12/2020.
//
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <cassert>
#include <ranges>
#include <algorithm>
#include <set>
#include <numeric>

namespace fs = std::filesystem;
namespace rg = std::ranges;

std::pair<int, std::vector<bool>> run_instructions(std::vector<std::string> const& inst)
{
    int accumulatedValue {0};
    std::vector<bool> visited (inst.size(), false);
    auto visitedPosition {visited.begin()};
    for (auto position {inst.begin()}; position != inst.end() && !*visitedPosition; ) {
        if (position->starts_with("jmp")) {
            auto const jump {
                position->at(4) == '+' ? std::stoi(position->substr(5)) : -std::stoi(position->substr(5))
            };
            *visitedPosition = true;
            std::advance(position, jump);
            std::advance(visitedPosition, jump);
            continue;
        }
        else if (position->starts_with("acc")) {
            auto const add {std::stoi(position->substr(5))};
            if (position->at(4) == '+') {
                accumulatedValue += add;
            }
            else {
                accumulatedValue -= add;
            }
        }
        *visitedPosition = true;
        ++position;
        ++visitedPosition;
    }

    return {accumulatedValue, visited};
}

bool find_breaking_index(size_t index,
                         std::vector<std::string> const& instructions)
{
    bool breaksLoop {true};

    std::set<size_t> checked;
    size_t idx {index};
    for ( ; idx != instructions.size(); ) {
        if (checked.contains(idx)) {
            breaksLoop = false;
            break;
        }
        checked.insert(idx);
        if (instructions[idx].starts_with("acc")) {
            ++idx;
        }
        else if (instructions[idx].starts_with("jmp")) {
            if (idx == index) {
                ++idx;
            }
            else {
                auto const jump {std::stoul(instructions[idx].substr(5))};
                idx += instructions[idx][4] == '+' ? jump : -jump;
                if (jump == 0) {
                    ++idx;
                }
            }
        }
        else {
            if (idx == index) { // from nop to jmp
                auto const jump {std::stoul(instructions[idx].substr(5))};
                idx += instructions[idx][4] == '+' ? jump : -jump;
                if (jump == 0) {
                    ++idx;
                }
            }
            else {
                ++idx;
            }
        }
    }

    return breaksLoop;
}

std::pair<int, std::vector<bool>> replay_break_loop(std::vector<std::string> const& instructions, std::vector<bool> const& visited)
{
    std::vector<size_t> allIndices (instructions.size(), 0);
    std::iota(allIndices.begin(), allIndices.end(), 0);
    auto const breakingIndex {
        std::find_if(allIndices.cbegin(), allIndices.cend(),
                     [&instructions, &visited]
                     (auto const& idx)
                     { return visited[idx] && find_breaking_index(idx, instructions); })
    };

    auto copyOfInstructions {instructions};
    if (copyOfInstructions[*breakingIndex].starts_with("acc")) {
        throw std::runtime_error{"Acc instruction shouldn't be changed"};
    }
    if (copyOfInstructions[*breakingIndex].starts_with("jmp")) {
        copyOfInstructions[*breakingIndex].replace(0, 3, "nop");
    }
    else {
        copyOfInstructions[*breakingIndex].replace(0, 3, "jmp");
    }

    return run_instructions(copyOfInstructions);
}

int main(int argc, char* argv[])
{
    auto const load_instructions {
        [](std::derived_from<std::istream> auto& is)
        {
            std::vector<std::string> lines;
            for (std::string instruction; is.good(); ) {
                std::getline(is, instruction);
                if (!instruction.empty()) {
                    lines.push_back(instruction);
                }
            }
            return lines;
        }
    };

    auto const instructions {
        [&argc, &argv, &load_instructions]()
        {
            if (argc > 1) {
                fs::path const filePath{argv[1]};
                if (fs::exists(filePath) && fs::is_regular_file(filePath)) {
                    std::ifstream fileStream{filePath};
                    return load_instructions(fileStream);
                }
                else {
                    return std::vector<std::string>{};
                }
            }
            else {
                return load_instructions(std::cin);
            }
        }()
    };

    auto const [result, visitedInstructions] {run_instructions(instructions)};
    assert(result == 1928);
    std::cout << "Accumulated value: " << result << '\n';

    std::cout << "Visited " << rg::count_if(visitedInstructions, [](auto const& i){ return i; })
              << " instructions out of " << instructions.size() << '\n';

    auto const [accumulatedWithoutLoop, rivisited] {replay_break_loop(instructions, visitedInstructions)};
    assert(accumulatedWithoutLoop == 1319);
    std::cout << "Accumulated after fix: " << accumulatedWithoutLoop << '\n';
    std::cout << "Visited after fix: " << rg::count_if(rivisited, [](auto const& i){ return i; }) << '\n';
}
