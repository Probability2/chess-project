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

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)



set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/libs/mygui")

# Подключаем директории для ImGui
include_directories(${IMGUI_DIR})
include_directories(${IMGUI_DIR}/backends)

# Указываем исходные файлы для ImGui
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    #${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    #${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Создаем библиотеку imgui
add_library(imgui STATIC ${IMGUI_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE imgui)


target_link_libraries(${PROJECT_NAME} PRIVATE glfw )

target_link_libraries(${PROJECT_NAME} PRIVATE glad)


target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
