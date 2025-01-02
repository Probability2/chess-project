add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE History)
target_link_libraries(${PROJECT_NAME} PRIVATE Piece)
target_link_libraries(${PROJECT_NAME} PRIVATE FindMate)
target_link_libraries(${PROJECT_NAME} PRIVATE Format)
target_link_libraries(${PROJECT_NAME} PRIVATE Game)
target_link_libraries(${PROJECT_NAME} PRIVATE Basic)
target_link_libraries(${PROJECT_NAME} PRIVATE BMPImage)
target_link_libraries(${PROJECT_NAME} PRIVATE Display)
target_link_libraries(${PROJECT_NAME} PRIVATE argparser)
target_link_libraries(${PROJECT_NAME} PRIVATE string_argument_class)
target_link_libraries(${PROJECT_NAME} PRIVATE int_argument_class)
target_link_libraries(${PROJECT_NAME} PRIVATE flag_class)
target_link_libraries(${PROJECT_NAME} PRIVATE Manager)
target_link_libraries(${PROJECT_NAME} PRIVATE Commands)
target_link_libraries(${PROJECT_NAME} PRIVATE Random)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

#include_directories(${SQLite3_INCLUDE_DIRS})

#target_link_libraries(${PROJECT_NAME} PRIVATE SQLite::SQLite3)


target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

