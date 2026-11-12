cmake_minimum_required(VERSION 3.16)
project(prime_checker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(prime_checker main.cpp)
add_executable(prime_benchmark benchmark.cpp)

enable_testing()
add_executable(prime_checker_tests tests.cpp)
add_test(NAME prime_checker_tests COMMAND prime_checker_tests)
