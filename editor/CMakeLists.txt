file(GLOB_RECURSE EDITOR_SRC CONFIGURE_DEPENDS "*.cpp" "*.hpp")

add_library(ImGUI STATIC
    C:/dev/libs/imgui-1.92.5-docking/imgui.cpp
    C:/dev/libs/imgui-1.92.5-docking/imgui_demo.cpp
    C:/dev/libs/imgui-1.92.5-docking/imgui_draw.cpp
    C:/dev/libs/imgui-1.92.5-docking/imgui_tables.cpp
    C:/dev/libs/imgui-1.92.5-docking/imgui_widgets.cpp
    C:/dev/libs/imgui-1.92.5-docking/backends/imgui_impl_glfw.cpp
    C:/dev/libs/imgui-1.92.5-docking/backends/imgui_impl_opengl3.cpp
)

target_include_directories(ImGUI PUBLIC
    C:/dev/libs/imgui-1.92.5-docking
    C:/dev/libs/imgui-1.92.5-docking/backends
)

target_compile_definitions(ImGUI PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)

target_link_libraries(ImGUI PUBLIC
    ${GLFW_LIBRARIES}
    opengl32
)

add_library(editor STATIC ${EDITOR_SRC})
target_include_directories(editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(editor PUBLIC engine ImGUI)