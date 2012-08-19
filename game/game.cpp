#include <scge/scge.hpp>

int main(int argc, char** argv) {
	//open a window
	window w("Hello World", 320, 240);

	//here is our main loop
	while(!w.key("escape")) {
		//check for key presses and other events
		w.poll();
	}
}
