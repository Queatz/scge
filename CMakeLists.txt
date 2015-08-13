# Main configuration file.

cmake_minimum_required(VERSION 2.8)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/)
set(GLM_INCLUDE_PATH "../glm/" CACHE STRING "GLM header files.")
set(PYTHON_INCLUDE_PATH "/usr/include/python3.4/" CACHE STRING "Python header files.")
set(FREETYPE_INCLUDE_PATH "/usr/include/freetype2/" CACHE STRING "Freetype header files.")

#link_directories(/usr/local/lib/)
include_directories(${PYTHON_INCLUDE_PATH})
include_directories(${FREETYPE_INCLUDE_PATH})
include_directories(${GLM_INCLUDE_PATH})

# Make the engine.
add_subdirectory(engine)
