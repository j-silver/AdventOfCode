//
// Created by giuseppe on 05/12/2020.
//

#ifndef ADVENTOFCODE_LINE_H
#define ADVENTOFCODE_LINE_H

#include <string>
#include <istream>

class Line {
public:
    Line() = default;

    friend std::istream& operator>>(std::istream& is, Line& line);
    friend std::ostream& operator<<(std::ostream& os, Line const& line);

    operator std::string() const;
    [[nodiscard]] std::string const& str() const;

private:
    std::string _line{};
};

#endif //ADVENTOFCODE_LINE_H
