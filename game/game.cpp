#include <scge/scge.hpp>

int main(int argc, char** argv) {
	//open a window
	window(320, 240);
	
	//set the window title
	window_title("Hello World");

	//here is our main loop
	while(!key("esc") && window_opened()) {
	
		//draw a point in the center of the screen
		point(160.0, 120.0);
	
		//make the drawing of the point visible on the screen
		swap();
	}
}
