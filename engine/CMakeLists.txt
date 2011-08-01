# Engine configuration file.

project(scge)
add_library(scge SHARED engine.cpp)

option(WITH_GRAPHICS "Enable graphics." ON)
option(WITH_SOUND "Enable sound." ON)
option(WITH_NETWORK "Enable networking." ON)
option(WITH_MATH "Enable advanced math." ON)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/conf.in ${CMAKE_CURRENT_SOURCE_DIR}/conf.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

if(${WITH_GRAPHICS})
	target_link_libraries(scge glfw GLEW SOIL freeimage freetype ftgl)
#	target_link_libraries(scge Xxf86vm Xext X11 GL pthread m /usr/local/lib/libglfw.a GLEW SOIL freeimage freetype /usr/local/lib/libftgl.a)
endif()

if(${WITH_SOUND})
	target_link_libraries(scge fluidsynth vorbisfile alure openal)
#	target_link_libraries(scge fluidsynth vorbisfile /usr/local/lib/libalure-static.a openal)
endif()

if(${WITH_NETWORK})
	target_link_libraries(scge enet)
#	target_link_libraries(scge /usr/local/lib/libenet.a)
endif()

install(TARGETS scge LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
install_files(/include/scge/ scge.hpp)
