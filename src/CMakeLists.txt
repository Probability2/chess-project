add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ChessLibrary)
target_link_libraries(${PROJECT_NAME} PRIVATE Types)
target_link_libraries(${PROJECT_NAME} PRIVATE Commands)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

