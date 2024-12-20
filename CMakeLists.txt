cmake_minimum_required(VERSION 3.12)

project(
    chess
    VERSION 1.0
    DESCRIPTION "My own C++ project"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)


add_subdirectory(lib)
add_subdirectory(parser)
add_subdirectory(puzzle)
add_subdirectory(engine)
add_subdirectory(src)
