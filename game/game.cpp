// Include the engine
#include "../engine/engine.hpp"

int main(int argc, char** argv) {
	// Open a display in windowed mode
	display d("window", 640, 480);
	// Init the keyboard for use
	keyboard k;
	// Init the mouse for use
	mouse m;
	
	// Open the display and set the window title
	d.on("Game");
	
	// While no one is pressing escape, q, or the X button on the window, run the game
	while(!k.key("esc") && !k.key("q") && d.opened()) {
		// Update display
		d.update();
	}
	
	// Exit time, close display
	d.off();
	
	// We exit successfully
	return EXIT_SUCCESS;
}
