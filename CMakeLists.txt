cmake_minimum_required(VERSION 3.17)
project(AdventOfCode)

option(NINJA_COLOURED_OUTPUT "Always produce coloured output with Ninja" TRUE)

# Force coloured output with Ninja
if (${NINJA_COLOURED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_compile_options (-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_compile_options (-fcolor-diagnostics)
    endif ()
endif ()


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)
set(CMAKE_CXX_EXTENSIONS false)

# Flags for Debug and Release targets
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -Wpedantic -Wshadow -Wconversion \
                           -Wshadow -Wswitch-enum -Weffc++ -ggdb3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native")

set(FIND_LIBRARY_USE_LIB64_PATHS true)

enable_testing()

find_package(GTest)

# set(CMAKE_CXX_CLANG_TIDY clang-tidy -p cmake-build-debug)

add_library(line line.cpp line.h)

add_executable(line_test line_test.cpp)
target_link_libraries(line_test PRIVATE line GTest::GTest)

add_test(NAME line_test COMMAND line_test)

add_executable(ReportRepair report_repair.cpp)

add_executable(PasswordPhilosophy password_philosophy.cpp)
target_link_libraries(PasswordPhilosophy PRIVATE line)

add_executable(TobogganTrajectory toboggan_trajectory.cpp)
target_link_libraries(TobogganTrajectory PRIVATE line)

add_executable(PassportProcessing passport_processing.cpp)

add_executable(BinaryBoarding binary_boarding.cpp)
target_link_libraries(BinaryBoarding PRIVATE line)

add_executable(CustomCustoms custom_customs.cpp)

add_executable(HandyHaversacks handy_haversacks.cpp)
target_link_libraries(HandyHaversacks PRIVATE line)