# Engine configuration file.

project(scge)
add_library(scge SHARED engine.cpp)

set(LIBRARY_LIST glfw GLEW SOIL freetype ftgl vorbisfile alure openal enet)# aubio
target_link_libraries(scge ${LIBRARY_LIST})

install(TARGETS scge LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
install_files(/include/scge/ scge.hpp)
