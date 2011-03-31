set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/)

project(game)
add_executable(game game.cpp)
target_link_libraries(game scge)
