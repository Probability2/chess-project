cmake_minimum_required(VERSION 3.20)

project(
    chess
    VERSION 1.0
    DESCRIPTION "My own C++ project"
    LANGUAGES CXX
)

# add_compile_options(-O3 -march=native)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# add_compile_options(-std=c++26)

if(WIN32) # Install dlls in the same directory as the executable on Windows
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

add_subdirectory(src)
add_subdirectory(lib)
add_subdirectory(parser)
add_subdirectory(core)

# include(FetchContent)

# FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
# FetchContent_MakeAvailable(json)

enable_testing()
add_subdirectory(tests)