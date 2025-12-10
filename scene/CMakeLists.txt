file(GLOB_RECURSE SCENE_SRC CONFIGURE_DEPENDS "*.cpp" "*.hpp")

add_library(scene STATIC ${SCENE_SRC})
target_include_directories(scene PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(scene PUBLIC engine)