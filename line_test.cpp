//
// Created by giuseppe on 05/12/2020.
//

#include "line.h"
#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>

GTEST_TEST(line_test, default_constructor)
{
    Line line;
}

GTEST_TEST(line_test, read_from_istream)
{
    std::istringstream is {"input"};
    Line line;
    is >> line;
}

GTEST_TEST(line_test, output_to_ostream)
{
    std::istringstream is {"input"};
    Line line;
    is >> line;
    std::fstream devNull {std::filesystem::path {"/dev/null"}};
    devNull << line;
}

GTEST_TEST(line_test, convert_to_string)
{
    std::istringstream is {"input"};
    Line line;
    is >> line;
    std::string lineString {line};
    GTEST_CHECK_(lineString == "input");
}

GTEST_TEST(line_test, default_contruct_to_empty)
{
    Line line;
    GTEST_CHECK_(std::string{line} == "");
}

GTEST_TEST(line_test, str)
{
    Line line;
    std::istringstream is {"input"};
    is >> line;
    GTEST_CHECK_(line.str() == "input");
}

GTEST_TEST(ranges_of_lines, read_lines_from_filestream)
{
    std::fstream file {std::filesystem::path {"../files/input1.txt"}};
    auto const lines {std::ranges::istream_view<Line>(file)};
}

GTEST_TEST(ranges_of_lines, output_stream)
{
    std::fstream file {std::filesystem::path {"../files/input1.txt"}};
    auto lines {std::ranges::istream_view<Line>(file)};
    std::fstream devNull {std::filesystem::path {"/dev/null"}};
    for (auto const& line : lines) {
        devNull << line << '\n';
    }
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}