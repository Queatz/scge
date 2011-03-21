#include "../engine/engine.hpp"

int main(int argc, char** argv) {
	//call initiate first
	initiate();

	//open a window
	window(320, 240);

	//set up a 2D display, mapped with 0,0 in the bottom-left
	orthographic(0.0, 320.0, 0.0, 240.0);

	//here is our main loop
	while(!key("esc") && window_opened()) {
	
		//draw a point in the center of the screen
		point(160.0, 120.0);
	
		//make the drawing of the point visible on the screen
		swap();
	}

	//finish up
	done();
}
