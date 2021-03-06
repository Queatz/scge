#include "conf.h"

#include "scge.hpp"

namespace scge {

#include "functions.cpp"

#ifdef WITH_GRAPHICS

Shikoba::Library * font_library = NULL;
int glfw_state = 0;

#include "font.cpp"
#include "display.cpp"
#include "window.cpp"
#include "program.cpp"
#include "pixelcache.cpp"
#include "image.cpp"
#include "vao.cpp"
#include "vbo.cpp"
#include "ibo.cpp"
#include "fbo.cpp"

#endif

#ifdef WITH_SOUND

std::vector<ALuint> loaded_sounds;
std::vector<ALuint> loaded_buffers;
int alure_state = 0;

int midi_status = 0;

fluid_settings_t * midi_settings = NULL;
fluid_synth_t * midi_synth = NULL;
fluid_audio_driver_t * midi_device = NULL;

#include "audio.cpp"
#include "microphone.cpp"
#include "soundbyte.cpp"
#include "buffer.cpp"
#include "sound.cpp"
#include "midi.cpp"

#endif

#ifdef WITH_NETWORK

bool connected = false;
int peercount = 0;
#include "network.cpp"

#endif

}
