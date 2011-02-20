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

struct Crop {
	bool use;
	float x, y, w, h;
};
// Types of resources.
#define NUM_BUFS 3
struct sound {
	sound(const char*, bool = false);
	~sound();
	
	void gain(float);
	void pitch(float);
	void pan(float);
	void repeat(bool);
	
	// Stream.
	void update();
	
	private:
	ALuint buffer[NUM_BUFS], source;
	alureStream* stream;
	bool is_stream, is_playing, looping;
	
	friend struct speaker;
};

struct image {
	image(const char*);
	image(int, int);
	// The width and height of the image.
	float w, h;
	
	void set(const char*);
	
	private:
	// The OpenGL texture ID,
	GLuint id;
	
	friend struct display;
	friend struct program;
	friend struct fbo;
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
	GLint s;
	
	friend struct program;
};

struct program {
	program();
	~program();
	
	void attach(shader*);
	void link();
	
	void uniform1i(const char*, int);
	void uniform1f(const char*, float);
	void uniform2f(const char*, float, float);
	void uniform3f(const char*, float, float, float);
	void uniform4f(const char*, float, float, float, float);
	
	void bind_image(const char*, int, image*);
	
	//private:
	GLhandleARB p;
	
	friend struct display;
};

struct fbo {
	display* d;
	image* i;

	fbo(display*);
	~fbo();
	
	private:
	GLuint id;
	
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
	
	// Use a portion of the image for drawing.
	void set_image_crop(int = NULL, int = NULL, int = NULL, int = NULL);
	
	// Draw an image on the display.
	void draw(float = 0, float = 0, float = 1, float = NULL, float = NULL, float = 0, float = NULL);
	
	// Write a font on the screen.
	void write(string, float = 0, float = 0);
	
	// Set color to use.
	void color(float, float, float, float = 1);
	
	// Sets the blending mode
	void blend_mode(const char*);
	
	// Enable/disable various things.
	void set(const char*, bool);
	
	// Set the point rendering size.
	void point_size(float);
	
	// Begin drawing certain things.
	void begin(const char*);
	
	// End drawing.
	void end();
	
	// draw a rectangle.
	void quad(float, float, float, float);
	
	// draw a mapped rectangle.
	void mquad(float, float, float, float);
	
	// draw a textured rectangle.
	void background_image(float = 0, float = 0, float = 0);
	
	// draw a point.
	void point(float, float);
	
	// use a shader program.
	void use_program(program*);
	
	// Apply a 2D filter.
	void apply_filter(program*);
	
	// Remove 2D filter.
	void apply_filter();
	
	// sleep for specified seconds.
	void sleep(float);
	
	// Use no shader program.
	void use_program();
	
	// Get time.
	float time();
	
	// Resolution of the display.
	int w, h;
	
	// Fullscreen.
	bool fs;
	
	fbo* framebuffer;
	program* filter;
	image* current_image;
	font* current_font;
	Crop image_crop;
};

struct timer {
	timer();
	
	void update();
	float delta();
	int tick();
	float total();
	int fps();
	
	private:
	float f;
	int t;
	float l;
	float s;
};

// Speaker

struct speaker {
	~speaker();
	
	void on();
	void off();
	
	// Play a sound.
	void play(sound*);
	void stop(sound*);
	void pause(sound*);
	
	friend struct sound;
};

// Mouse
struct mouse {
	mouse();
	
	// Hide the mouse.
	void hide();
	
	// Show the mouse.
	void show();
	
	int x, y;
	
	bool clicked(short);
	bool clicked(const char *);
	
	int wheel();
	int scroll();
	
	// Fill x and y.
	void update();
	
	private:
	int wheel_pos;
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
