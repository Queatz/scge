#include "conf.h"

#include "scge.hpp"

namespace scge {

#include "functions.cpp"

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

#ifdef WITH_PYTHON
#include "python.cpp"
#endif

}
