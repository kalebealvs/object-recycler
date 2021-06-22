cmake_minimum_required(VERSION 3.20.0)
project(object_recycler)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE -03)
set(PROJECT_CXX_FLAGS -Wall -Wextra -Wshadow -Wpedantic -march=native)

include_directories(include)

add_subdirectory(libs/googletest)

enable_testing()
add_subdirectory(tests)