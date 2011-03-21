//#define WITH_NETWORK
//#define WITH_3D

#include <cstdlib>
#include <cstring>
#include <vector>
#include <stack>
#include <list>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdarg>
#include <ctype.h> // toupper()

#include <GL/glew.h> // OpenGL extentions
#include <GL/glfw.h> // Graphics

#include <SOIL/SOIL.h>

#include <FTGL/ftgl.h> // Font rendering
#include <AL/alure.h> // Sound
#include <aubio/aubio.h> // Pitch, etc

#ifdef WITH_NETWORK
#include <enet/enet.h> // Networking
#endif

#ifdef WITH_3D
#include <eigen3/Eigen/Eigen> // Advanced math
#endif

struct box {
	box(float = 0.0, float = 0.0);
	
	float w, h;
};

struct ibox {
	ibox(int = 0, int = 0);
	
	int w, h;
};

struct offset {
	offset(float = 0.0, float = 0.0);
	
	float x, y;
};

struct rect {
	rect(float = 0.0, float = 0.0, float = 0.0, float = 0.0);
	
	float x, y, w, h;
};

struct irect {
	irect(int = 0, int = 0, int = 0, int = 0);
	
	int x, y, w, h;
};

struct crop {
	crop(bool = true, float = 0, float = 0, float = 0, float = 0);
	
	bool use;
	float x, y, w, h;
};

struct icrop {
	icrop(bool = true, int = 0, int = 0, int = 0, int = 0);
	
	bool use;
	int x, y, w, h;
};

struct rgba {
	rgba(float = 0.0, float = 0.0, float = 0.0, float = 1.0);
	
	float r, g, b, a;
};

#define NUM_BUFS 3

struct sound {
	sound(const char*, bool = false);
	~sound();
	
	void gain(float);
	void pitch(float);
	void pan(float);
	void repeat(bool = true);
	
	void update();
	
	ALuint buffer[NUM_BUFS], source;
	alureStream* stream;
	bool is_stream, is_playing, looping;
};

struct pixelcache {
	pixelcache(int, int);
	pixelcache(const pixelcache&);
	~pixelcache();
	
	int width, height;
	GLubyte *data;
};

struct image {
	image(const char*);
	image(int, int, bool = false);
	~image();
	
	//void save(const char*);
	
	float width, height;
	
	void set(const char*);
	
	void from_pixelcache(pixelcache* = NULL);
	void refresh_pixel_cache();
	rgba pixel(int, int);
	
	GLuint id;
	pixelcache* cache;
};

struct font {
	font(const char*, float = 12);
	~font();
	
	void size(float);
	void push_size();
	void pop_size();
	
	float width(const char *);
	float height(const char *);
	
	FTFont* data;
	float size_default;
	
	std::stack<float, std::list<float> > size_stack;
};

struct shader {
	shader(const char*, const char*);
	~shader();
	
	void compile();
	
	GLint id;
};

struct program {
	program();
	~program();
	
	void attach(shader*);
	void link();
	
	void uniform_int(const char*, int);
	void uniform_float(const char*, float);
	void uniform_float(const char*, float, float);
	void uniform_float(const char*, float, float, float);
	void uniform_float(const char*, float, float, float, float);
	
	void uniform_image(const char*, int, image*);
	
	GLhandleARB id;
};

struct fbo {
	image* buffer;

	fbo(int, int, bool = false);
	~fbo();
	GLuint id;
};

bool initiate();
void done();

bool window(int, int, bool = false);
void close_window();

void window_title(const char*);
bool window_opened();
bool window_active();
float window_timer();
void rest(float);

ibox display_dimensions();
ibox window_dimensions();
void position_window(int, int);

void vsync(bool = true);
void swap();
void poll();
void screenshot(const char*);

void orthographic(float, float, float, float, float = -1.0, float = 1.0);
void perspective(float, float, float, float, float = 0.1, float = 1000.0);

rgba pixel(int, int);

void polygon_mode(const char*);
void enable(const char*, bool = true);

void clear_color(float, float, float, float = 1.0);
void clear();
void point_size(float);
void line_width(float);
void line_stipple(int, const char* = NULL);

void blend_color(float, float, float, float = 1.0);

void color(rgba);
void color(float, float, float, float = 1.0);
void push_color();
void pop_color();
rgba get_color();

void use_fbo(fbo*);
void use_fbo();
void push_fbo();
void pop_fbo();
fbo* get_fbo();

void use_image(image*);
void push_image();
void pop_image();
image* get_image();

void use_font(font*);
void push_font();
void pop_font();
font* get_font();

void image_crop(float, float, float, float);
void image_crop();
void push_image_crop();
void pop_image_crop();
crop get_image_crop();

void scissor(int, int, int, int);
void scissor();
void push_scissor();
void pop_scissor();
icrop get_scissor();

void blend_mode(const char*);
void blend_mode();
void push_blend_mode();
void pop_blend_mode();
const char* get_blend_mode();

void use_program(program*);
void use_program();
void push_program();
void pop_program();
program* get_program();

void push_matrix();
void pop_matrix();
void translate(float, float);
void rotate(float);
void scale(float, float);


void begin(const char*);
void end();

void draw(float = 0.0, float = 0.0, float = 0.0, float = 0.0, float = 0.0, float = 0.0, float = 0.0);
void idraw(float = 0.0, float = 0.0, float = 0.0, float = 0.0, float = 0.0, float = 0.0, float = 0.0);
void write(const char*, float = 0.0, float = 0.0, bool = false);

void background_image(float = 0.0, float = 0.0, float = 1.0);

void quad(float, float, float, float);
void quad();
void mquad(float, float, float, float);
void mquad();

void iquad(float, float, float, float);
void imquad(float, float, float, float);

void point(float, float);
void line(float, float, float, float);

void ipoint(float, float);
void iline(float, float, float, float);

void portion(int, int, int, int);

offset mouse_position();

void mouse(bool = false);
bool button(short);
bool button(const char *);
int wheel();
void move_mouse(int, int);

bool key(const char*);

bool audio();
void audio_off();

void clear_audio();

// Update queued sounds and streams
void update_audio();

// Play a sound
void play(sound*, bool = false, bool = false);
void stop(sound*);
void pause(sound*);
/*
void microphone();
void microphone_off();
void microphone_update();
sound* ms();
*/
#ifdef WITH_NETWORK

// Server

struct connection {
	~connection();
	
	void on();
	void off();
};

struct server {
	// Create a new server on port, with max connections of, with max numbr of channels, with limited downstream, with limited upstream
	server(int = 2000, int = 32, int = 1, int = 0, int = 0);
	// Close the server
	~server();
	
	ENetEvent service(int = 2000);
	
	ENetAddress address;
	ENetHost* host;
};

struct client {
	// Create a new client with max connections of, with max numbr of channels, with limited downstram, with limited upstream
	client(int = 1, int = 1, int = 0, int = 0);
	// Close the client
	~client();
	
	void send(const char* = "", int = 0);
	void commune();
	
	// Connect to server, on port, with number of channels
	void connect(const char* = "localhost", int = 2000, int = 1);
	void disconnect();
	
	// Service within timeout
	ENetEvent service(int = 2000);
	
	ENetHost* host;
	ENetPeer* peer;
};

#endif

// Other

// Seed random
void seed_rnd(int);

// Random 0-1
float rnd();
