add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ChessLibrary)
target_link_libraries(${PROJECT_NAME} PRIVATE parser)
target_link_libraries(${PROJECT_NAME} PRIVATE Commands)

target_include_directories(${PROJECT_NAME} PRIVATE "C:/SFML-3.1.0/include")
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(chess PRIVATE
    "C:/SFML-3.1.0/lib/libsfml-graphics.a"
    "C:/SFML-3.1.0/lib/libsfml-window.a"
    "C:/SFML-3.1.0/lib/libsfml-system.a"
)

if (WIN32)
  add_custom_command(TARGET chess POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "C:/SFML-3.1.0/bin/sfml-graphics-3.dll"
            "C:/SFML-3.1.0/bin/sfml-window-3.dll"
            "C:/SFML-3.1.0/bin/sfml-system-3.dll"
            "${CMAKE_BINARY_DIR}"
    )
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

