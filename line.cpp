//
// Created by giuseppe on 05/12/2020.
//

#include "line.h"

std::istream &operator>>(std::istream &is, Line& line)
{
    return std::getline(is, line._line);
}

std::ostream& operator<<(std::ostream& os, Line const& line)
{
    return os << line._line;
}

std::string const& Line::str() const
{
    return _line;
}

Line::operator std::string() const
{
    return _line;
}
