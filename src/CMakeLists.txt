add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ChessLibrary)
target_link_libraries(${PROJECT_NAME} PRIVATE parser)
target_link_libraries(${PROJECT_NAME} PRIVATE Commands)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

