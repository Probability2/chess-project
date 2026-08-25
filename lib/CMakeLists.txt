add_library(ChessLibrary 
    History.cpp
    Position.cpp
    Game.cpp
    FileManager.cpp
    MoveGenerator.cpp
    Move.cpp
)

# target_link_libraries(ChessLibrary 
#     PRIVATE 
#         nlohmann_json::nlohmann_json
#     PUBLIC
#         GTest::gtest
# )

target_include_directories(ChessLibrary 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(types)
