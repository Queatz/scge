# Engine configuration file.

project(engine)
add_library(engine engine.cpp)
target_link_libraries(engine ${LIBRARY_LIST})
