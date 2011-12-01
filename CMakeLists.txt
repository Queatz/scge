# Main configuration file.

cmake_minimum_required(VERSION 2.8)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/)

#link_directories(/usr/local/lib/)
include_directories(/usr/include/freetype2/)
include_directories(/usr/include/python3.2/)

# Make the engine.
add_subdirectory(engine)
