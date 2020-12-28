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
set(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -Wpedantic -Wshadow -Wconversion \
                            -Wshadow -Wswitch-enum -Weffc++ -ggdb3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native")

set(FIND_LIBRARY_USE_LIB64_PATHS true)

enable_testing()

find_package(GTest)

include(conan-build/conanbuildinfo.cmake)
conan_basic_setup()

# set(CMAKE_CXX_CLANG_TIDY clang-tidy -p cmake-build-debug)

add_library(line SHARED line.cpp line.h)

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

add_executable(HandheldHalting handheld_halting.cpp)

add_executable(EncodingError encoding_error.cpp)

add_executable(AdapterArray adapter_array.cpp
                            depth_first_search.cpp
                            depth_first_search.h
                            groups.cpp
                            groups.h)

add_library(adapter_tests
            SHARED adapter_arrays_test.cpp
                   depth_first_search.cpp
                   depth_first_search_fork.cpp
                   bifurcations.h bifurcations.cpp
                   groups.h groups.cpp)
set_property(TARGET adapter_tests PROPERTY LINKER_LANGUAGE CXX)

add_executable(Adapters_Test catch-test-main.cpp)
target_link_libraries(Adapters_Test PRIVATE line adapter_tests)
set_property(TARGET Adapters_Test PROPERTY LINKER_LANGUAGE CXX)

add_test(NAME Adapters_Test COMMAND Adapters_Test)

add_executable(SeatingSystem seating_system.cpp)
target_link_libraries(SeatingSystem PRIVATE tbb)

add_executable(RainRisk rain_risk.cpp)

add_executable(ShuttleSearch shuttle_search.cpp shuttle_search.h shuttle_search_functions.cpp)

add_library(shuttle_tests SHARED shuttle_tests.cpp shuttle_search.h shuttle_search_functions.cpp)
set_property(TARGET shuttle_tests PROPERTY LINKER_LANGUAGE CXX)

add_executable(Shuttle_Test catch-test-main.cpp)
target_link_libraries(Shuttle_Test PRIVATE shuttle_tests)
set_property(TARGET Shuttle_Test PROPERTY LINKER_LANGUAGE CXX)

add_test(NAME Shuttle_Test COMMAND Shuttle_Test)

add_library(docking_tests SHARED docking_tests.cpp docking.cpp)
set_property(TARGET docking_tests PROPERTY LINKER_LANGUAGE CXX)

add_executable(Docking_Test catch-test-main.cpp)
target_link_libraries(Docking_Test PRIVATE docking_tests)
set_property(TARGET Docking_Test PROPERTY LINKER_LANGUAGE CXX)

add_test(NAME Docking_Test COMMAND Docking_Test)

add_executable(DockingData docking_data.cpp docking.h docking.cpp)