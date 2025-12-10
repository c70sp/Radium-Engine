file(GLOB_RECURSE GAME_SRC CONFIGURE_DEPENDS "*.cpp" "*.hpp")

add_library(game STATIC ${GAME_SRC})
target_include_directories(game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(game PUBLIC engine)