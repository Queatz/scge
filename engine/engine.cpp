#include "conf.h"

#include "scge.hpp"

#include "functions.cpp"
#include "types.cpp"
#include "extra.cpp"

#ifdef WITH_GRAPHICS
#include "display.cpp"
#endif

#ifdef WITH_SOUND
#include "audio.cpp"
#include "midi.cpp"
#endif

#ifdef WITH_NETWORK
#include "network.cpp"
#endif
