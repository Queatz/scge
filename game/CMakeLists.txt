set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/)

project(game)

include_directories(/usr/include/freetype2/)

add_executable(game game.cpp)
target_link_libraries(game scge)
