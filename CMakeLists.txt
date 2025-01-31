cmake_minimum_required(VERSION 3.10)
project(main)

set(CMAKE_TOOLCHAIN_FILE "C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(glad_DIR "C:/dev/vcpkg/installed/x64-mingw-dynamic/share/glad")
set(glfw3_DIR "C:/dev/vcpkg/installed/x64-windows/share/glfw3")
set(glm_DIR "C:/dev/vcpkg/installed/x64-windows/share/glm")

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

file(GLOB SRC_FILES "src/*.cpp") # GLOB_RECURSE falls nested
add_executable(main ${SRC_FILES})

target_include_directories(main PRIVATE include)

target_link_libraries(main PRIVATE glad::glad glfw glm::glm dwmapi)