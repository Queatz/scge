#define WITH_GRAPHICS
#define WITH_SOUND
#define WITH_NETWORK
#include <scge/scge.hpp>

int main(int argc, char** argv) {
	//open a window
	scge::window w("Hello World");

	//here is our main loop
	while(!w.key("escape")) {
		//check for key presses and other events
		scge::poll();
	}
}
