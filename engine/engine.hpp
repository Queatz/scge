//#define WITH_NETWORK
//#define WITH_3D

#include <cstdlib>
#include <cstring>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdarg>

#include <GL/glew.h> // OpenGL extentions
#include <GL/glfw.h> // Graphics
#include <ctype.h> // toupper()

#include <SOIL/SOIL.h>

#include <FTGL/ftgl.h> // Font rendering
#include <AL/alure.h> // Sound

#ifdef WITH_NETWORK
#include <enet/enet.h> // Networking
#endif

#ifdef WITH_3D
#include <eigen3/Eigen/Eigen> // Advanced math
#endif

// Init some beforehand.
struct display;
struct speaker;

using namespace std;

struct Size {
	float w, h;
};

// Types of resources.
struct sound {
	sound(const char*);
	~sound();
	
	void gain(float);
	void pitch(float);
	void pan(float);
	void repeat(bool);
	
	private:
	ALuint buffer, source;
	
	friend struct speaker;
};

struct image {
	image(const char*);
	// The OpenGL texture ID,
	int id;
	// The width and height of the image.
	float w, h;
};

struct text {};

struct font {
	font(const char*, float = 12);
	~font();
	
	// Set the font size.
	void size(float);
	
	float width(const char *);
	float height(const char *);
	
	private:
	// The font.
	FTFont* f;
	
	friend struct display;
};

struct shader {
	shader(const char*, const char*);
	~shader();
	
	void compile();
	
	private:
	GLenum s;
	
	friend struct program;
};

struct program {
	program();
	~program();
	
	void attach(shader*);
	void link();
	
	private:
	GLenum p;
	
	friend struct display;
};

// Display
struct display {
	// Optional display initiation parameters.
	display(const char* = "", int = 0, int = 0);
	~display();
	
	// Set up the display.
	bool on(const char* = NULL);
	
	// Close the display.
	void off();
	
	// Set the window title.
	void title(const char*);
	
	// Check if the display is open.
	bool opened();
	
	// Time since display was opened.
	float timer();
	
	// Get display dimentions.
	Size dimensions();
	
	// Clear the display.
	void clear();
	
	// Swap buffer of display.
	void update();
	
	// Set the image to draw with.
	void use_image(image*);
	
	// Set the font to write with.
	void use_font(font*);
	
	// Draw an image_resource on the display.
	void draw(float = 0, float = 0, float = 1, float = NULL, float = NULL, float = 0, float = NULL);
	
	// Write a font on the screen.
	void write(string, float = 0, float = 0);
	
	// Set color to use.
	void color(float, float, float, float = 1);
	
	// Sets the blending mode
	void blend_mode(const char*);
	
	// Enable/disable various things.
	void set(const char*, bool);
	
	// Change image filter.
	void image_set(const char*);
	
	// Set the point rendering size.
	void point_size(float);
	
	// Begin drawing certain things.
	void begin(const char*);
	
	// End drawing.
	void end();
	
	// draw a rectangle.
	void quad(float, float, float, float);
	
	// draw a textured rectangle.
	void background_image(float = 0, float = 0, float = 0);
	
	// draw a point.
	void point(float, float);
	
	// Use a shader program.
	void use_program(program*);
	
	// Use no shader program.
	void use_program();
	
	// Get time.
	float time();
	
	// Resolution of the display.
	int w, h;
	
	// Fullscreen.
	bool fs;
	
	// Percentage to shade the display with blackness.
	float black;
	
	private:
	image* current_image;
	font* current_font;
};

struct timer {
	timer();
	
	void update();
	float delta();
	int tick();
	int fps();
	
	private:
	float f;
	int t;
	float l;
};

// Speaker

struct speaker {
	~speaker();
	
	void on();
	void off();
	
	// Play a sound.
	void play(sound*);
	
	friend struct sound;
};

// Mouse
struct mouse {
	// Hide the mouse.
	void hide();
	
	// Show the mouse.
	void show();
};

// Keyboard
struct keyboard {
	// Get key state.
	bool key(const char*);
};

#ifdef WITH_NETWORK

// Server

struct connection {
	~connection();
	
	void on();
	void off();
};

struct server {
	// Create a new server on port, with max connections of, with max numbr of channels, with limited downstream, with limited upstream.
	server(int = 2000, int = 32, int = 1, int = 0, int = 0);
	// Close the server.
	~server();
	
	ENetEvent service(int = 2000);
	
	private:
	ENetAddress address;
	ENetHost* host;
};

struct client {
	// Create a new client with max connections of, with max numbr of channels, with limited downstram, with limited upstream.
	client(int = 1, int = 1, int = 0, int = 0);
	// Close the client.
	~client();
	
	void send(const char* = "", int = 0);
	void commune();
	
	// Connect to server, on port, with number of channels.
	void connect(const char* = "localhost", int = 2000, int = 1);
	void disconnect();
	
	// Service within timeout.
	ENetEvent service(int = 2000);
	
	//private:
	ENetHost* host;
	ENetPeer* peer;
};

#endif

// Other

// Seed random.
void seed_rnd(int);

// Random 0-1.
float rnd();
