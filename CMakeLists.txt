cmake_minimum_required(VERSION 3.10)
project(main)

set(CMAKE_TOOLCHAIN_FILE "C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(engine)
add_subdirectory(editor)
add_subdirectory(scene)
add_subdirectory(game)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE engine editor scene game)
