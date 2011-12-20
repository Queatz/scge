#include <scge/scge.hpp>

int main(int argc, char** argv) {
	//open a window
	window("Hello World", 320, 240);

	//here is our main loop
	while(!key("esc") && window_opened()) {
		//check for key presses and other events
		poll();
	}
}
