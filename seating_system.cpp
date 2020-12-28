//
// Created by giuseppe on 15/12/2020.
//

#include <vector>
#include <iostream>
#include <set>
#include <numeric>
#include <execution>
#include <cassert>

enum class Seat { Empty, Occupied, Floor };

class Layout {
public:
    explicit Layout(std::istream& is) : _grid {}
    {
        for (std::string line; is.good(); line.clear()) {
            std::getline(is, line);
            if (! line.empty()) {
                std::vector<Seat> row;
                std::transform(line.cbegin(), line.cend(), std::back_inserter(row),
                       [](auto const& c)
                               {
                                    if (c == '#') {
                                        return Seat::Occupied;
                                    }
                                    else if (c == 'L') {
                                        return Seat::Empty;
                                    }
                                    else if (c == '.') {
                                        return Seat::Floor;
                                    }
                                    else {
                                        throw std::invalid_argument{"Invalid character"};
                                    }
                               });
                _grid.push_back(row);
            }
        }
    }

    friend std::ostream& operator<<(std::ostream& os, Layout const& l);
    friend bool operator==(Layout const& lhs, Layout const& rhs);

    Layout& evolve()
    {
        std::vector<std::vector<Seat>> evolvedGrid (_grid.size(), std::vector<Seat>(_grid[0].size()));
        for (size_t row {0}; row < _grid.size(); ++row) {
            for (size_t col {0}; col < _grid.front().size(); ++col) {
                auto const adj {adjacent(row, col)};
                if (_grid[row][col] == Seat::Empty &&
                    std::count_if(adj.cbegin(), adj.cend(),
                                  [this](auto const& pos){ return _grid[pos.first][pos.second] == Seat::Occupied; }) == 0)
                {
                    evolvedGrid[row][col] = Seat::Occupied;
                }
                else if (_grid[row][col] == Seat::Occupied &&
                        std::count_if(adj.cbegin(), adj.cend(),
                                      [this](auto const& pos){ return _grid[pos.first][pos.second] == Seat::Occupied; }) >= 4)
                {
                    evolvedGrid[row][col] = Seat::Empty;
                }
                else {
                    evolvedGrid[row][col] = _grid[row][col];
                }
            }
        }

        std::swap(_grid, evolvedGrid);
        return *this;
    }

    Layout& evolve_differently()
    {
        std::vector<std::vector<Seat>> evolvedGrid (_grid.size(), std::vector<Seat>(_grid[0].size()));
        for (size_t row {0}; row < _grid.size(); ++row) {
            for (size_t col {0}; col < _grid.front().size(); ++col) {
                if (_grid[row][col] == Seat::Empty && count_all(row, col) == 0) {
                    evolvedGrid[row][col] = Seat::Occupied;
                }
                else if (_grid[row][col] == Seat::Occupied && count_all(row, col) >= 5) {
                    evolvedGrid[row][col] = Seat::Empty;
                }
                else {
                    evolvedGrid[row][col] = _grid[row][col];
                }
            }
        }

        std::swap(_grid, evolvedGrid);
        return *this;
    }

    long occupied() const
    {
        return std::transform_reduce(std::execution::par_unseq,
                                     _grid.cbegin(), _grid.cend(), 0L,
                               [](auto init, auto const& sumOverRow)
                                        {
                                            return init += sumOverRow;
                                        },
                               [](auto const& row) {
                                            return std::count_if(row.cbegin(), row.cend(),
                                                                [](auto const& seat) {
                                                                    return seat == Seat::Occupied;
                                                                });
                                        });
    }

private:
    std::set<std::pair<size_t, size_t>> adjacent(size_t row, size_t column) const
    {
        std::set<std::pair<size_t, size_t>> adj;

        if (column != 0) {
            adj.insert({row, column - 1});
        }
        if (row != 0 && column != 0) {
            adj.insert({row - 1, column - 1});
        }
        if (row != 0) {
            adj.insert({row - 1, column});
        }
        if (row != 0 && column != _grid.front().size() - 1) {
            adj.insert({row - 1, column + 1});
        }
        if (column != _grid.front().size() - 1) {
            adj.insert({row, column + 1});
        }
        if (row != _grid.size() - 1 && column != _grid.front().size() -1) {
            adj.insert({row + 1, column + 1});
        }
        if (row != _grid.size() - 1) {
            adj.insert({row + 1, column});
        }
        if (row != _grid.size() - 1 && column != 0) {
            adj.insert({row + 1, column - 1});
        }
        
        return adj;
    }

    long count_horizontal(size_t row, size_t col)
    {
        long counter {0};
        for (int c {static_cast<int>(col)-1}; c >= 0; --c) {
            if (_grid[row][static_cast<size_t>(c)] == Seat::Occupied) {
                ++counter;
                break;
            }
            else if (_grid[row][static_cast<size_t>(c)] == Seat::Empty) {
                break;
            }
        }
        for (size_t c {col+1}; c < _grid.front().size(); ++c) {
            if (_grid[row][c] == Seat::Occupied) {
                ++counter;
                break;
            }
            else if (_grid[row][c] == Seat::Empty) {
                break;
            }
        }
        return counter;
    }

    long count_vertical(size_t row, size_t col)
    {
        long counter {0};
        for (int r {static_cast<int>(row)-1}; r >= 0; --r) {
            if (_grid[static_cast<size_t>(r)][col] == Seat::Occupied) {
                ++counter;
                break;
            }
            else if (_grid[static_cast<size_t>(r)][col] == Seat::Empty) {
                break;
            }
        }
        for (size_t r {row+1}; r < _grid.size(); ++r) {
            if (_grid[r][col] == Seat::Occupied) {
                ++counter;
                break;
            }
            else if (_grid[r][col] == Seat::Empty) {
                break;
            }
        }
        return counter;
    }

    long count_diagonal(size_t row, size_t col)
    {
        long counter {0};
        for (size_t r {row+1}, c {col+1}; r < _grid.size() && c < _grid.front().size(); ++r, ++c) {
            if (_grid[r][c] == Seat::Occupied) {
                ++counter;
                break;
            }
            else if (_grid[r][c] == Seat::Empty) {
                break;
            }
        }
        for (int r {static_cast<int>(row)+1}, c {static_cast<int>(col)-1};
             r < static_cast<int>(_grid.size()) && c >= 0;
             ++r, --c)
        {
            if (_grid[static_cast<size_t>(r)][static_cast<size_t>(c)] == Seat::Occupied) {
                ++counter;
                break;
            }
            else if (_grid[static_cast<size_t>(r)][static_cast<size_t>(c)] == Seat::Empty) {
                break;
            }
        }
        for (int r {static_cast<int>(row)-1}, c {static_cast<int>(col)+1};
             r >= 0 && c < static_cast<int>(_grid.front().size());
             --r, ++c)
        {
            if (_grid[static_cast<size_t>(r)][static_cast<size_t>(c)] == Seat::Occupied) {
                ++counter;
                break;
            }
            else if (_grid[static_cast<size_t>(r)][static_cast<size_t>(c)] == Seat::Empty) {
                break;
            }
        }
        for (int r {static_cast<int>(row)-1}, c {static_cast<int>(col)-1};
             r >= 0 && c >= 0;
             --r, --c)
        {
            if (_grid[static_cast<size_t>(r)][static_cast<size_t>(c)] == Seat::Occupied) {
                ++counter;
                break;
            }
            else if (_grid[static_cast<size_t>(r)][static_cast<size_t>(c)] == Seat::Empty) {
                break;
            }
        }

        return counter;
    }

    long count_all(size_t row, size_t col)
    {
        return count_horizontal(row, col) + count_vertical(row, col) + count_diagonal(row, col);
    }

    std::vector<std::vector<Seat>> _grid;
};

bool operator==(Layout const& lhs, Layout const& rhs)
{
    return lhs._grid == rhs._grid;
}

std::ostream& operator<<(std::ostream& os, Layout const& l)
{
    for (auto const& row : l._grid) {
        for (auto const& state : row) {
            if (state == Seat::Empty) {
                os << 'L';
            }
            else if (state == Seat::Occupied) {
                os << '#';
            }
            else {
                os << '.';
            }
        }
        os << '\n';
    }
    return os;
}

int main()
{
    Layout layout {std::cin};
    auto layoutCopy {layout};
    for (auto old {layout}; old != layout.evolve(); old = layout) {}
    auto const answer1 {layout.occupied()};
    assert(answer1 == 2113);
    std::cout << answer1 << '\n';

    for (auto old {layoutCopy}; old != layoutCopy.evolve_differently(); old = layoutCopy) {}
    auto const answer2 {layoutCopy.occupied()};
    assert(answer2 == 1865);
    std::cout << answer2 << '\n';

}