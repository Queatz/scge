# Engine configuration file.

project(engine)
add_library(engine engine.cpp)
target_link_libraries(engine glfw GLEW SOIL freetype ftgl vorbisfile alure openal) #enet
