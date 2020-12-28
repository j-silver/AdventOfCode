//
// Created by giuseppe on 16/12/2020.
//

#include <iostream>
#include <complex>

using namespace std::literals::complex_literals;

class Position {
public:
    Position() : _waypoint {10.0 + 1i} {}

    Position& action(std::string const& a)
    {
        if (a.empty()) {
            return *this;
        }

        double const length {std::stod(a.substr(1))};

        char const dir {a.front()};
        if (dir == 'N') {
            _coordinates += length * 1i;
        }
        else if (dir == 'E') {
            _coordinates += length;
        }
        else if (dir == 'S') {
            _coordinates += -1i * length;
        }
        else if (dir == 'W') {
            _coordinates += -length;
        }
        else if (dir == 'R') {
            int const halfPi {static_cast<int>(length/90.0)};
            _direction *= std::pow(-1i, halfPi);
        }
        else if (dir == 'L') {
            int const halfPi {static_cast<int>(length/90.0)};
            _direction *= std::pow(1i, halfPi);
        }
        else if (dir == 'F') {
            _coordinates += length * _direction;
        }

        return *this;
    }

    Position& action_with_waypoint(std::string const& move)
    {
        if (move.empty()) {
            return *this;
        }

        auto const length {std::stoi(move.substr(1))};
        double const offset {static_cast<double>(length)};
        auto const squareAngles {length/90};

        char const dir {move.front()};
        if (dir == 'N') {
            _waypoint._coordinates += 1i * offset;
        }
        else if (dir == 'E') {
            _waypoint._coordinates += offset;
        }
        else if (dir == 'S') {
            _waypoint._coordinates += -1i * offset;
        }
        else if (dir == 'W') {
            _waypoint._coordinates += -offset;
        }
        else if (dir == 'R') {
            _waypoint._coordinates = _coordinates + (_waypoint._coordinates - _coordinates) * std::pow(-1i, squareAngles);
        }
        else if (dir == 'L') {
            _waypoint._coordinates = _coordinates + (_waypoint._coordinates - _coordinates) * std::pow(1i, squareAngles);
        }
        else if (dir == 'F') {
            auto const shift {_waypoint._coordinates - _coordinates};
            _coordinates += offset * (_waypoint._coordinates - _coordinates);
            _waypoint._coordinates = _coordinates + shift;
        }

        return *this;
    }

    unsigned long long manhattan_distance() const
    {
        return static_cast<unsigned long long>(std::abs(std::real(_coordinates)) + std::abs(std::imag(_coordinates)));
    }

    auto coordinates() const
    {
        return _coordinates;
    }

private:
    std::complex<double> _coordinates {0};
    std::complex<double> _direction {1};

    struct Waypoint {
        std::complex<double> _coordinates {10.0 + 1i};
    } _waypoint;
};

int main()
{
    Position position1, position2;
    for (std::string move; std::cin.good(); ) {
        std::getline(std::cin, move);
        position1.action(move);
        position2.action_with_waypoint(move);
    }

    std::cout << '\n' << position1.manhattan_distance() << '\n';
    std::cout << position2.manhattan_distance() << '\n';
}