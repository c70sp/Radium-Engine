# Switched to MSYS2, before I had a weird mix of vcpkg and MSYS2, which caused a lot of headaches
# Now the CMakeLists.txt is fully made for MSYS2 (https://chat.deepseek.com/a/chat/s/18e5829a-0f84-4db3-8562-a1e006330765)

cmake_minimum_required(VERSION 3.10)
project(engine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GLFW REQUIRED glfw3)
pkg_check_modules(ASSIMP REQUIRED assimp)
pkg_check_modules(BULLET REQUIRED bullet)

find_path(GLM_INCLUDE_DIR glm/glm.hpp PATHS /mingw64/include)

# vvvvvvvvvvvvvvvvvvvv GLAD SHIT vvvvvvvvvvvvvvvvvvvv
# vcpkg's GLAD does not have any extensions, for anisotropic filtering I need a custom build which has that as an extension
# Current GLAD extensions (thanks ChatGPT for the great summaries of them)
# GL_ARB_compute_shader                     (for using compute shaders, duh)
# GL_ARB_debug_output                       (Enables easy GPU-side error logging, glDebugMessageCallback for realtime GPU error logging)
# GL_ARB_shader_image_load_store            (Lets shaders read/write arbitrary texture pixels. Great for GPGPU or deferred rendering)
# GL_ARB_shader_storage_buffer_object       (Required for complex GPU data structures or compute pipelines)
# GL_EXT_texture_filter_anisotropic         (Improves texture quality at oblique angles)
# Could also possibly add the following extensions:
# GL_ARB_direct_state_access                (DSA, modify GL objects without binding them)
# GL_ARB_texture_storage                    (Cleaner texture allocation (glTexStorage*), prevents resizing errors)
# GLAD version is 4.6, profile is compatibility
add_library(glad STATIC "C:/dev/libs/glad/src/glad.c")
target_include_directories(glad PUBLIC "C:/dev/libs/glad/include")
# ^^^^^^^^^^^^^^^^^^^^ GLAD SHIT ^^^^^^^^^^^^^^^^^^^^

# vvvvvvvvvvvvvvvvvvvv FREETYPE SHIT vvvvvvvvvvvvvvvvvvvv
# Currently set to debug, set to release when finishing project.
# Uses shared lib, meaning I need to include .dll and .dll.a  when sharing the project with others
# Could also generate static lib, then it gets included directly into the executable, makes it bigger but removes dependancy hell
# https://chat.deepseek.com/a/chat/s/2027917e-be3d-4712-bed2-78df9ae34831 <- last couple of messages (Okay so this is for shared:...)
set(FREETYPE_ROOT_DIR "C:/dev/libs/freetype-2.14.1")
set(FREETYPE_INCLUDE_DIR "${FREETYPE_ROOT_DIR}/install-debug/include/freetype2")
set(FREETYPE_DLL "${FREETYPE_ROOT_DIR}/install-debug/bin/libfreetyped.dll")
set(FREETYPE_LIB "${FREETYPE_ROOT_DIR}/install-debug/lib/libfreetyped.dll.a")

add_library(freetype_custom SHARED IMPORTED)
set_target_properties(freetype_custom PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${FREETYPE_INCLUDE_DIR}"
    IMPORTED_IMPLIB "${FREETYPE_LIB}"
    IMPORTED_LOCATION "${FREETYPE_DLL}"
)
# ^^^^^^^^^^^^^^^^^^^^ FREETYPE SHIT ^^^^^^^^^^^^^^^^^^^^

file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS "*.cpp" "*.hpp")
add_library(engine STATIC ${ENGINE_SRC})
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(engine PUBLIC glad ${GLFW_LINK_LIBRARIES} ${ASSIMP_LINK_LIBRARIES} ${BULLET_LINK_LIBRARIES} freetype_custom)
target_include_directories(engine PUBLIC ${GLFW_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIRS} ${BULLET_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})