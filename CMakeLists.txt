# Main configuration file.

cmake_minimum_required(VERSION 2.8)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/)

# Make the engine.
add_subdirectory(engine)

# Make the game.
project(game)
add_executable(game game/game.cpp)
target_link_libraries(game engine)
