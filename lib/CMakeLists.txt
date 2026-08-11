add_library(ChessLibrary 
    History.cpp
    Piece.cpp
    Position.cpp
    Game.cpp
    FileManager.cpp
)

target_link_libraries(ChessLibrary 
    PRIVATE 
        nlohmann_json::nlohmann_json
    PUBLIC
        GTest::gtest
)