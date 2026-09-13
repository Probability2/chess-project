add_library(Types INTERFACE 
    Attacks.hpp
    Bitboard.hpp
    Piece.hpp
    EvaluationTables.hpp
)

target_include_directories(Types 
    INTERFACE 
        ${CMAKE_CURRENT_SOURCE_DIR}
)