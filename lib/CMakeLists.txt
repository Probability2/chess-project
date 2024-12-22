add_library(History History.cpp)
add_library(Piece ChessPiece.cpp)
add_library(Format ChessFormat.cpp)
add_library(Game ChessGame.cpp)
add_library(Display DisplayBoard.cpp)
add_library(BMPImage BMPImage.cpp)

target_link_libraries(History PRIVATE nlohmann_json::nlohmann_json)
