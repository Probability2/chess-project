cmake_minimum_required(VERSION 3.5)

project(
    chess
    VERSION 1.0
    DESCRIPTION "My own C++ project"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

if(WIN32) # Install dlls in the same directory as the executable on Windows
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

add_subdirectory(src)
add_subdirectory(lib)
add_subdirectory(parser)
add_subdirectory(puzzle)
add_subdirectory(engine)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)


add_subdirectory( ${PROJECT_SOURCE_DIR}/libs/GLFW  )

add_subdirectory(libs/glad)

