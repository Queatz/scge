# Engine configuration file.

project(scge)
add_library(scge SHARED engine.cpp)

option(WITH_GRAPHICS "Enable graphics." ON)
option(WITH_SOUND "Enable sound." ON)
option(WITH_NETWORK "Enable networking." ON)
option(WITH_MATH "Enable advanced math." ON)
option(WITH_PYTHON "Enable Python specifics." ON)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/conf.in ${CMAKE_CURRENT_SOURCE_DIR}/conf.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

if(${WITH_GRAPHICS})
	target_link_libraries(scge GL GLU /usr/local/lib/libglfw.so GLEW freeimage freetype ftgl)
endif()

if(${WITH_SOUND})
	target_link_libraries(scge fluidsynth vorbisfile alure openal aubio)
endif()

if(${WITH_NETWORK})
	target_link_libraries(scge enet)
endif()

install(TARGETS scge LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
install_files(/include/scge/ scge.hpp)
